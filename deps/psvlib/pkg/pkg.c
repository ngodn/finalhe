#include "pkg.h"

#include "aes.h"
#include "pkg_out.h"
#include "pkg_utils.h"
#include "pkg_zrif.h"
// #include "pkg_sys.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <stddef.h>

#define _FILE_OFFSET_BITS 64
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <stdarg.h>
#include <stddef.h>


#define PKG_HEADER_SIZE 192
#define PKG_HEADER_EXT_SIZE 64
#define ZIP_MAX_FILENAME 1024

// https://wiki.henkaku.xyz/vita/Packages#AES_Keys
static const uint8_t pkg_vita_2[] = { 0xe3, 0x1a, 0x70, 0xc9, 0xce, 0x1d, 0xd7, 0x2b, 0xf3, 0xc0, 0x62, 0x29, 0x63, 0xf2, 0xec, 0xcb };
static const uint8_t pkg_vita_3[] = { 0x42, 0x3a, 0xca, 0x3a, 0x2b, 0xd5, 0x64, 0x9f, 0x96, 0x86, 0xab, 0xad, 0x6f, 0xd8, 0x80, 0x1f };
static const uint8_t pkg_vita_4[] = { 0xaf, 0x07, 0xfd, 0x59, 0x65, 0x25, 0x27, 0xba, 0xf1, 0x33, 0x89, 0x66, 0x8b, 0x17, 0xd9, 0xea };

pkg_output_func _output_func = sys_output;
pkg_error_func _error_func = sys_error;

static int gStdoutRedirected;

void sys_output_init(void)
{
    gStdoutRedirected = !isatty(STDOUT_FILENO);
}

void sys_output_done(void)
{
}

void sys_output(void *a, const char* msg, ...)
{
    va_list arg;
    va_start(arg, msg);
    vfprintf(stdout, msg, arg);
    va_end(arg);
}

void sys_error(void *a, const char* msg, ...)
{
    va_list arg;
    va_start(arg, msg);
    vfprintf(stderr, msg, arg);
    va_end(arg);

    exit(EXIT_FAILURE);
}

static void sys_mkdir_real(const char* path)
{
    if (mkdir(path, S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH) < 0)
    {
        if (errno != EEXIST)
        {
            _error_func(NULL, "ERROR: cannot create '%s' folder\n", path);
        }
    }
}

sys_file sys_open(const char* fname, uint64_t* size)
{
    int fd = open(fname, O_RDONLY);
    if (fd < 0)
    {
        _error_func(NULL, "ERROR: cannot open '%s' file\n", fname);
    }

    struct stat st;
    if (fstat(fd, &st) != 0)
    {
        _error_func(NULL, "ERROR: cannot get size of '%s' file\n", fname);
    }
    *size = st.st_size;

    return (void*)(intptr_t)fd;
}

sys_file sys_create(const char* fname)
{
    int fd = open(fname, O_RDWR | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
    if (fd < 0)
    {
        _error_func(NULL, "ERROR: cannot create '%s' file\n", fname);
    }

    return (void*)(intptr_t)fd;
}

void sys_close(sys_file file)
{
    if (close((int)(intptr_t)file) != 0)
    {
        _error_func(NULL, "ERROR: failed to close file\n");
    }
}

void sys_read(sys_file file, uint64_t offset, void* buffer, uint32_t size)
{
    ssize_t read = pread((int)(intptr_t)file, buffer, size, offset);
    if (read < 0 || read != (ssize_t)size)
    {
        _error_func(NULL, "ERROR: failed to read %u bytes from file\n", size);
    }
}

void sys_write(sys_file file, uint64_t offset, const void* buffer, uint32_t size)
{
    ssize_t wrote = pwrite((int)(intptr_t)file, buffer, size, offset);
    if (wrote < 0 || wrote != (ssize_t)size)
    {
        _error_func(NULL, "ERROR: failed to read %u bytes from file\n", size);
    }
}

void sys_mkdir(const char* path)
{
    char* last = strrchr(path, '/');
    if (last)
    {
        *last = 0;
        sys_mkdir(path);
        *last = '/';
    }
    sys_mkdir_real(path);
}

void* sys_realloc(void* ptr, size_t size)
{
    void* result = NULL;
    if (!ptr && size)
    {
        result = malloc(size);
    }
    else if (ptr && !size)
    {
        free(ptr);
        return NULL;
    }
    else if (ptr && size)
    {
        result = realloc(ptr, size);
    }
    else
    {
        _error_func(NULL, "ERROR: internal error, wrong sys_realloc usage\n");
    }

    if (!result)
    {
        _error_func(NULL, "ERROR: out of memory\n");
    }

    return result;
}

void sys_vstrncat(char* dst, size_t n, const char* format, ...)
{
    char temp[1024];

    va_list args;
    va_start(args, format);
    vsnprintf(temp, sizeof(temp), format, args);
    va_end(args);

    strncat(dst, temp, n - strlen(dst) - 1);
}

static uint64_t out_size;
static uint32_t out_next;

void sys_output_progress_init(void *arg, uint64_t size)
{
    out_size = size;
    out_next = 0;
}

void sys_output_progress(void *arg, uint64_t progress)
{
    if (gStdoutRedirected)
    {
        return;
    }

    uint32_t now = (uint32_t)(progress * 100 / out_size);
    if (now >= out_next)
    {
        _output_func(arg, "[*] unpacking... %u%%\r", now);
        out_next = now + 1;
    }
}

static pkg_output_progress_init_func _output_progress_init_func = sys_output_progress_init;
static pkg_output_progress_func _output_progress_func = sys_output_progress;
static void *_output_arg = NULL;

// http://vitadevwiki.com/vita/System_File_Object_(SFO)_(PSF)#Internal_Structure
// https://github.com/TheOfficialFloW/VitaShell/blob/1.74/sfo.h#L29
static int parse_sfo_content(const uint8_t* sfo, uint32_t sfo_size, char* category, char* title, char* content, char* min_version, char* pkg_version)
{
    if (get32le(sfo) != 0x46535000)
    {
        _error_func(_output_arg, "ERROR: incorrect sfo signature\n");
        return -1;
    }

    uint32_t keys = get32le(sfo + 8);
    uint32_t values = get32le(sfo + 12);
    uint32_t count = get32le(sfo + 16);

    int title_index = -1;
    int content_index = -1;
    int category_index = -1;
    int minver_index = -1;
    int pkgver_index = -1;
    for (uint32_t i = 0; i < count; i++)
    {
        if (i * 16 + 20 + 2 > sfo_size)
        {
            _error_func(_output_arg, "ERROR: sfo information is too small\n");
            return -1;
        }

        char* key = (char*)sfo + keys + get16le(sfo + i * 16 + 20);
        if (strcmp(key, "TITLE") == 0)
        {
            if (title_index < 0)
            {
                title_index = (int)i;
            }
        }
        else if (strcmp(key, "STITLE") == 0)
        {
            title_index = (int)i;
        }
        else if (strcmp(key, "CONTENT_ID") == 0)
        {
            content_index = (int)i;
        }
        else if (strcmp(key, "CATEGORY") == 0)
        {
            category_index = (int)i;
        }
        else if (strcmp(key, "PSP2_DISP_VER") == 0)
        {
            minver_index = (int)i;
        }
        else if (strcmp(key, "APP_VER") == 0)
        {
            pkgver_index = (int)i;
        }
    }

    if (title_index < 0)
    {
        _error_func(_output_arg, "ERROR: cannot find title from sfo file, pkg is probably corrupted\n");
        return -1;
    }

    char* value = (char*)sfo + values + get32le(sfo + title_index * 16 + 20 + 12);
    size_t i;
    size_t max = 255;
    for (i = 0; i<max && *value; i++, value++)
    {
        if ((*value >= 32 && *value < 127 && strchr("<>\"/\\|?*", *value) == NULL) || (uint8_t)*value >= 128)
        {
            if (*value == ':')
            {
                *title++ = ' ';
                *title++ = '-';
                max--;
            }
            else
            {
                *title++ = *value;
            }
        }
        else if (*value == 10)
        {
            *title++ = ' ';
        }
    }
    *title = 0;

    if (content_index >= 0 && content)
    {
        value = (char*)sfo + values + get32le(sfo + content_index * 16 + 20 + 12);
        while (*value)
        {
            *content++ = *value++;
        }
        *content = 0;
    }

    if (category_index >= 0)
    {
        value = (char*)sfo + values + get32le(sfo + category_index * 16 + 20 + 12);
        while (*value)
        {
            *category++ = *value++;
        }
    }
    *category = 0;

    if (minver_index >= 0 && min_version)
    {
        value = (char*)sfo + values + get32le(sfo + minver_index * 16 + 20 + 12);
        if (*value == '0')
        {
            value++;
        }
        while (*value)
        {
            *min_version++ = *value++;
        }
        if (min_version[-1] == '0')
        {
            min_version[-1] = 0;
        }
        else
        {
            *min_version = 0;
        }
    }

    if (pkgver_index >= 0 && pkg_version)
    {
        value = (char*)sfo + values + get32le(sfo + pkgver_index * 16 + 20 + 12);
        if (*value == '0')
        {
            value++;
        }
        while (*value)
        {
            *pkg_version++ = *value++;
        }
        *pkg_version = 0;
    }
    return 0;
}

static int parse_sfo(sys_file f, uint64_t sfo_offset, uint32_t sfo_size, char* category, char* title, char* content, char* min_version, char* pkg_version)
{
    uint8_t sfo[16 * 1024];
    if (sfo_size < 16)
    {
        _error_func(_output_arg, "ERROR: sfo information is too small\n");
        return -1;
    }
    if (sfo_size > sizeof(sfo))
    {
        _error_func(_output_arg, "ERROR: sfo information is too big, pkg file is probably corrupted\n");
        return -1;
    }
    sys_read(f, sfo_offset, sfo, sfo_size);

    return parse_sfo_content(sfo, sfo_size, category, title, content, min_version, pkg_version);
}

static const char* get_region(const char* id)
{
    if (memcmp(id, "PCSE", 4) == 0 || memcmp(id, "PCSA", 4) == 0 ||
        memcmp(id, "NPNA", 4) == 0)
    {
        return "USA";
    }
    else if (memcmp(id, "PCSF", 4) == 0 || memcmp(id, "PCSB", 4) == 0 ||
             memcmp(id, "NPOA", 4) == 0)
    {
        return "EUR";
    }
    else if (memcmp(id, "PCSC", 4) == 0 || memcmp(id, "VCJS", 4) == 0 || 
             memcmp(id, "PCSG", 4) == 0 || memcmp(id, "VLJS", 4) == 0 ||
             memcmp(id, "VLJM", 4) == 0 || memcmp(id, "NPPA", 4) == 0)
    {
        return "JPN";
    }
    else if (memcmp(id, "VCAS", 4) == 0 || memcmp(id, "PCSH", 4) == 0 ||
             memcmp(id, "VLAS", 4) == 0 || memcmp(id, "PCSD", 4) == 0 ||
             memcmp(id, "NPQA", 4) == 0)
    {
        return "ASA";
    }
    else
    {
        return "unknown region";
    }
}


typedef enum {
    PKG_TYPE_VITA_APP,
    PKG_TYPE_VITA_DLC,
    PKG_TYPE_VITA_PATCH,
    PKG_TYPE_VITA_PSM,
} pkg_type;

static int use_sys_output = 1;

int pkg_dec(const char *pkgname, const char *target_dir, const char *zrif)
{
    if (use_sys_output) sys_output_init();
    _output_func(_output_arg, "[*] loading...\n");

    uint64_t pkg_size;
    sys_file pkg = sys_open(pkgname, &pkg_size);

    uint8_t pkg_header[PKG_HEADER_SIZE + PKG_HEADER_EXT_SIZE];
    sys_read(pkg, 0, pkg_header, sizeof(pkg_header));

    if (get32be(pkg_header) != 0x7f504b47 || get32be(pkg_header + PKG_HEADER_SIZE) != 0x7F657874)
    {
        _error_func(_output_arg, "ERROR: not a pkg file\n");
        return -1;
    }

    // http://www.psdevwiki.com/ps3/PKG_files
    uint64_t meta_offset = get32be(pkg_header + 8);
    uint32_t meta_count = get32be(pkg_header + 12);
    uint32_t item_count = get32be(pkg_header + 20);
    uint64_t total_size = get64be(pkg_header + 24);
    uint64_t enc_offset = get64be(pkg_header + 32);
    uint64_t enc_size = get64be(pkg_header + 40);
    const uint8_t* iv = pkg_header + 0x70;
    int key_type = pkg_header[0xe7] & 7;

    if (pkg_size < total_size)
    {
        _error_func(_output_arg, "ERROR: pkg file is too small\n");
        return -1;
    }
    if (pkg_size < enc_offset + item_count * 32)
    {
        _error_func(_output_arg, "ERROR: pkg file is too small\n");
        return -1;
    }

    uint32_t content_type = 0;
    uint32_t sfo_offset = 0;
    uint32_t sfo_size = 0;
    uint32_t items_offset = 0;
    uint32_t items_size = 0;

    for (uint32_t i = 0; i < meta_count; i++)
    {
        uint8_t block[16];
        sys_read(pkg, meta_offset, block, sizeof(block));

        uint32_t type = get32be(block + 0);
        uint32_t size = get32be(block + 4);

        if (type == 2)
        {
            content_type = get32be(block + 8);
        }
        else if (type == 13)
        {
            items_offset = get32be(block + 8);
            items_size = get32be(block + 12);
        }
        else if (type == 14)
        {
            sfo_offset = get32be(block + 8);
            sfo_size = get32be(block + 12);
        }

        meta_offset += 2 * sizeof(uint32_t) + size;
    }

    pkg_type type;

    if (content_type == 0x15)
    {
        type = PKG_TYPE_VITA_APP;
    }
    else if (content_type == 0x16)
    {
        type = PKG_TYPE_VITA_DLC;
    }
    else if (content_type == 0x18 || content_type == 0x1d)
    {
        type = PKG_TYPE_VITA_PSM;
    }
    else
    {
        _error_func(_output_arg, "ERROR: unsupported content type 0x%x", content_type);
    }

    uint8_t main_key[16];
    if (key_type == 2)
    {
        aes_context key;
        aes_init(&key, pkg_vita_2, 128);
        aes_ecb_encrypt(&key, iv, main_key);
    }
    else if (key_type == 3)
    {
        aes_context key;
        aes_init(&key, pkg_vita_3, 128);
        aes_ecb_encrypt(&key, iv, main_key);
    }
    else if (key_type == 4)
    {
        aes_context key;
        aes_init(&key, pkg_vita_4, 128);
        aes_ecb_encrypt(&key, iv, main_key);
    }

    aes_context key;
    aes_init(&key, main_key, 128);

    char content[256];
    char title[256];
    char category[256];
    char min_version[256];
    char pkg_version[256];
    const char* id = content + 7;
    const char* id2 = id + 13;

    // first 512 - for vita games - https://github.com/TheOfficialFloW/NoNpDrm/blob/v1.1/src/main.c#L42
    // 1024 is used for PSM
    uint8_t rif[1024];
    uint32_t rif_size = 0;

    if (type == PKG_TYPE_VITA_PSM)
    {
        memcpy(content, pkg_header + 0x30, 0x30);
        rif_size = 1024;
    }
    else // Vita APP, DLC, PATCH or THEME
    {
        if (parse_sfo(pkg, sfo_offset, sfo_size, category, title, content, min_version, pkg_version) < 0)
            return -1;
        rif_size = 512;
            
        if (type == PKG_TYPE_VITA_APP && strcmp(category, "gp") == 0)
        {
            type = PKG_TYPE_VITA_PATCH;
        }
    }

    if (type != PKG_TYPE_VITA_PATCH && zrif != NULL)
    {
        zrif_decode(zrif, rif, rif_size);
        const char* rif_contentid = (char*)rif + (type == PKG_TYPE_VITA_PSM ? 0x50 : 0x10);
        if (strncmp(rif_contentid, content, 0x30) != 0)
        {
            _error_func(_output_arg, "ERROR: zRIF content id '%s' doesn't match pkg '%s'\n", rif_contentid, content);
        }
    }

    char root[1024];
    if (type == PKG_TYPE_VITA_DLC)
    {
        snprintf(root, sizeof(root), "%s [%.9s] [%s] [DLC-%s]", title, id, get_region(id), id2);
        _output_func(_output_arg, "[*] unpacking Vita DLC\n");
    }
    else if (type == PKG_TYPE_VITA_PATCH)
    {
        snprintf(root, sizeof(root), "%s [%.9s] [%s] [PATCH] [v%s]", title, id, get_region(id), pkg_version);
        _output_func(_output_arg, "[*] unpacking Vita PATCH\n");
    }
    else if (type == PKG_TYPE_VITA_PSM)
    {
        snprintf(root, sizeof(root), "%.9s [%s] [PSM]", id, get_region(id));
        _output_func(_output_arg, "[*] unpacking Vita PSM\n");
    }
    else if (type == PKG_TYPE_VITA_APP)
    {
        snprintf(root, sizeof(root), "%s [%.9s] [%s]", title, id, get_region(id));
        _output_func(_output_arg, "[*] unpacking Vita APP\n");
    }
    else
    {
        assert(0);
        _error_func(_output_arg, "ERROR: unsupported type\n");
    }

    if (target_dir == NULL)
        root[0] = 0;
    else
        strcpy(root, target_dir);

    if (type == PKG_TYPE_VITA_DLC)
    {
        sys_vstrncat(root, sizeof(root), "addcont");
        out_add_folder(root);

        sys_vstrncat(root, sizeof(root), "/%.9s", id);
        out_add_folder(root);

        sys_vstrncat(root, sizeof(root), "/%s", id2);
        out_add_folder(root);
    }
    else if (type == PKG_TYPE_VITA_PATCH)
    {
        sys_vstrncat(root, sizeof(root), "patch");
        out_add_folder(root);

        sys_vstrncat(root, sizeof(root), "/%.9s", id);
        out_add_folder(root);
    }
    else if (type == PKG_TYPE_VITA_PSM)
    {
        sys_vstrncat(root, sizeof(root), "psm");
        out_add_folder(root);

        sys_vstrncat(root, sizeof(root), "/%.9s", id);
        out_add_folder(root);
    }
    else if (type == PKG_TYPE_VITA_APP)
    {
        sys_vstrncat(root, sizeof(root), "app");
        out_add_folder(root);

        sys_vstrncat(root, sizeof(root), "/%.9s", id);
        out_add_folder(root);
    }
    else
    {
        assert(0);
        _error_func(_output_arg, "ERROR: unsupported type\n");
    }

    char path[1024];

    int sce_sys_package_created = 0;

    _output_progress_init_func(_output_arg, pkg_size);

    for (uint32_t item_index = 0; item_index < item_count; item_index++)
    {
        uint8_t item[32];
        uint64_t item_offset = items_offset + item_index * 32;
        sys_read(pkg, enc_offset + item_offset, item, sizeof(item));
        aes_ctr_xor(&key, iv, item_offset / 16, item, sizeof(item));

        uint32_t name_offset = get32be(item + 0);
        uint32_t name_size = get32be(item + 4);
        uint64_t data_offset = get64be(item + 8);
        uint64_t data_size = get64be(item + 16);
        uint8_t psp_type = item[24];
        uint8_t flags = item[27];

        assert(name_offset % 16 == 0);
        assert(data_offset % 16 == 0);

        if (pkg_size < enc_offset + name_offset + name_size ||
            pkg_size < enc_offset + data_offset + data_size)
        {
            _error_func(_output_arg, "ERROR: pkg file is too short, possibly corrupted\n");
        }

        if (name_size >= ZIP_MAX_FILENAME)
        {
            _error_func(_output_arg, "ERROR: pkg file contains file with very long name\n");
        }

        const aes_context* item_key = &key;

        char name[ZIP_MAX_FILENAME];
        sys_read(pkg, enc_offset + name_offset, name, name_size);
        aes_ctr_xor(item_key, iv, name_offset / 16, (uint8_t*)name, name_size);
        name[name_size] = 0;

        // _output_func(_output_arg, "[%u/%u] %s\n", item_index + 1, item_count, name);

        if (flags == 4 || flags == 18)
        {
            if (type == PKG_TYPE_VITA_PSM)
            {
                // skip "content/" prefix
                char* slash = strchr(name, '/');
                if (slash != NULL)
                {
                    snprintf(path, sizeof(path), "%s/RO/%s", root, name + 8);
                    out_add_folder(path);
                }
            }
            else if (type == PKG_TYPE_VITA_APP || type == PKG_TYPE_VITA_DLC || type == PKG_TYPE_VITA_PATCH)
            {
                snprintf(path, sizeof(path), "%s/%s", root, name);
                out_add_folder(path);

                if (strcmp("sce_sys/package", name) == 0)
                {
                    sce_sys_package_created = 1;
                }
            }
        }
        else
        {
            int decrypt = 1;
            if ((type == PKG_TYPE_VITA_APP || type == PKG_TYPE_VITA_DLC || type == PKG_TYPE_VITA_PATCH) && strcmp("sce_sys/package/digs.bin", name) == 0)
            {
                // TODO: is this really needed?
                if (!sce_sys_package_created)
                {
                    snprintf(path, sizeof(path), "%s/sce_sys/package", root);
                    out_add_folder(path);

                    sce_sys_package_created = 1;
                }
                snprintf(name, sizeof(name), "%s", "sce_sys/package/body.bin");
                decrypt = 0;
            }

            if (type == PKG_TYPE_VITA_PSM)
            {
                // skip "content/" prefix
                snprintf(path, sizeof(path), "%s/RO/%s", root, name + 8);
            }
            else
            {
                snprintf(path, sizeof(path), "%s/%s", root, name);
            }

            uint64_t offset = data_offset;

            out_begin_file(path);
            while (data_size != 0)
            {
                uint8_t PKG_ALIGN(16) buffer[1 << 16];
                uint32_t size = (uint32_t)min64(data_size, sizeof(buffer));
                _output_progress_func(_output_arg, enc_offset + offset);
                sys_read(pkg, enc_offset + offset, buffer, size);

                if (decrypt)
                {
                    aes_ctr_xor(item_key, iv, offset / 16, buffer, size);
                }

                out_write(buffer, size);
                offset += size;
                data_size -= size;
            }
            out_end_file();
        }
    }

    _output_func(_output_arg, "[*] unpacking completed\n");

    if (type == PKG_TYPE_VITA_APP || type == PKG_TYPE_VITA_DLC || type == PKG_TYPE_VITA_PATCH)
    {
        if (!sce_sys_package_created)
        {
            _output_func(_output_arg, "[*] creating sce_sys/package\n");
            snprintf(path, sizeof(path), "%s/sce_sys/package", root);
            out_add_folder(path);
        }

        _output_func(_output_arg, "[*] creating sce_sys/package/head.bin\n");
        snprintf(path, sizeof(path), "%s/sce_sys/package/head.bin", root);

        out_begin_file(path);
        uint64_t head_size = enc_offset + items_size;
        uint64_t head_offset = 0;
        while (head_size != 0)
        {
            uint8_t PKG_ALIGN(16) buffer[1 << 16];
            uint32_t size = (uint32_t)min64(head_size, sizeof(buffer));
            sys_read(pkg, head_offset, buffer, size);
            out_write(buffer, size);
            head_size -= size;
            head_offset += size;
        }
        out_end_file();

        _output_func(_output_arg, "[*] creating sce_sys/package/tail.bin\n");
        snprintf(path, sizeof(path), "%s/sce_sys/package/tail.bin", root);

        out_begin_file(path);
        uint64_t tail_offset = enc_offset + enc_size;
        while (tail_offset != pkg_size)
        {
            uint8_t PKG_ALIGN(16) buffer[1 << 16];
            uint32_t size = (uint32_t)min64(pkg_size - tail_offset, sizeof(buffer));
            sys_read(pkg, tail_offset, buffer, size);
            out_write(buffer, size);
            tail_offset += size;
        }
        out_end_file();

        _output_func(_output_arg, "[*] creating sce_sys/package/stat.bin\n");
        snprintf(path, sizeof(path), "%s/sce_sys/package/stat.bin", root);

        uint8_t stat[768] = { 0 };
        out_begin_file(path);
        out_write(stat, sizeof(stat));
        out_end_file();
    }

    if ((type == PKG_TYPE_VITA_APP || type == PKG_TYPE_VITA_DLC || type == PKG_TYPE_VITA_PSM) && zrif != NULL)
    {
        if (type == PKG_TYPE_VITA_PSM)
        {
            _output_func(_output_arg, "[*] creating RO/License\n");
            snprintf(path, sizeof(path), "%s/RO/License", root);
            out_add_folder(path);

            _output_func(_output_arg, "[*] creating RO/License/FAKE.rif\n");
            snprintf(path, sizeof(path), "%s/RO/License/FAKE.rif", root);
        }
        else
        {
            _output_func(_output_arg, "[*] creating sce_sys/package/work.bin\n");
            snprintf(path, sizeof(path), "%s/sce_sys/package/work.bin", root);
        }

        out_begin_file(path);
        out_write(rif, rif_size);
        out_end_file();
    }

    if (type == PKG_TYPE_VITA_PSM)
    {
        _output_func(_output_arg, "[*] creating RW\n");
        snprintf(path, sizeof(path), "%s/RW", root);
        out_add_folder(path);

        _output_func(_output_arg, "[*] creating RW/Documents\n");
        snprintf(path, sizeof(path), "%s/RW/Documents", root);
        out_add_folder(path);

        _output_func(_output_arg, "[*] creating RW/Temp\n");
        snprintf(path, sizeof(path), "%s/RW/Temp", root);
        out_add_folder(path);

        _output_func(_output_arg, "[*] creating RW/System\n");
        snprintf(path, sizeof(path), "%s/RW/System", root);
        out_add_folder(path);

        _output_func(_output_arg, "[*] creating RW/System/content_id\n");
        snprintf(path, sizeof(path), "%s/RW/System/content_id", root);
        out_begin_file(path);
        out_write(pkg_header + 0x30, 0x30);
        out_end_file();

        _output_func(_output_arg, "[*] creating RW/System/pm.dat\n");
        snprintf(path, sizeof(path), "%s/RW/System/pm.dat", root);

        uint8_t pm[1 << 16] = { 0 };
        out_begin_file(path);
        out_write(pm, sizeof(pm));
        out_end_file();
    }

    sys_close(pkg);

    if (type == PKG_TYPE_VITA_APP || type == PKG_TYPE_VITA_PATCH)
    {
        _output_func(_output_arg, "[*] minimum fw version required: %s\n", min_version);
    }

    _output_func(_output_arg, "[*] done!\n");
    if (use_sys_output) sys_output_done();
    return 0;
}

void dummy_out(void *arg, const char* msg, ...) {}
void dummy_error(void *arg, const char* msg, ...) {}
void dummy_progress_init(void *arg, uint64_t size) {}
void dummy_progress(void *arg, uint64_t progress) {}

void pkg_disable_output() {
    pkg_set_func(dummy_out, dummy_error, dummy_progress_init, dummy_progress, NULL);
}

void pkg_enable_sys_output() {
    pkg_set_func(sys_output, sys_error, sys_output_progress_init, sys_output_progress, NULL);
}

void pkg_set_func(pkg_output_func out, pkg_error_func err,
    pkg_output_progress_init_func proginit,
    pkg_output_progress_func prog, void *arg) {
    use_sys_output = 0;
    _output_arg = arg;
    if (out != NULL)
        _output_func = out;
    else
        _output_func = dummy_out;
    if (err != NULL)
        _error_func = err;
    else
        _error_func = dummy_error;
    if (proginit != NULL)
        _output_progress_init_func = proginit;
    else
        _output_progress_init_func = dummy_progress_init;
    if (prog != NULL)
        _output_progress_func = prog;
    else
        _output_progress_func = dummy_progress;
}
