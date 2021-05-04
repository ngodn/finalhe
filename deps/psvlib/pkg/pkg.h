#pragma once

#include <stdint.h>
#include <stddef.h>


#ifdef __cplusplus
extern "C" {
#endif

typedef void (*pkg_output_func)(void *arg, const char* msg, ...);
typedef void (*pkg_error_func)(void *arg, const char* msg, ...);
typedef void (*pkg_output_progress_init_func)(void *arg, uint64_t size);
typedef void (*pkg_output_progress_func)(void *arg, uint64_t progress);

int pkg_dec(const char *pkgname, const char *target_dir, const char *zrif);
void pkg_disable_output();
void pkg_enable_sys_output();
void pkg_set_func(pkg_output_func out, pkg_error_func err,
                 pkg_output_progress_init_func proginit,
                 pkg_output_progress_func prog, void *arg);
                 
// correctly outputs utf8 string
void sys_output_init(void);
void sys_output_done(void);
void sys_output(void *arg, const char* msg, ...);
void sys_error(void *arg, const char* msg, ...);

void sys_output_progress_init(void *arg, uint64_t size);
void sys_output_progress(void *arg, uint64_t progress);

typedef void* sys_file;

void sys_mkdir(const char* path);

sys_file sys_open(const char* fname, uint64_t* size);
sys_file sys_create(const char* fname);
void sys_close(sys_file file);
void sys_read(sys_file file, uint64_t offset, void* buffer, uint32_t size);
void sys_write(sys_file file, uint64_t offset, const void* buffer, uint32_t size);

// if !ptr && size => malloc
// if ptr && !size => free
// if ptr && size => realloc
void* sys_realloc(void* ptr, size_t size);

void sys_vstrncat(char* dst, size_t n, const char* format, ...);  

#ifdef __cplusplus
}
#endif
