Push the h-encore exploit for PS VITA andPS TV through latest macOS

## Credits

see [CREDITS.md](CREDITS.md)


# Final h-encore
# MacOS Big Sur 11.3.1

## Build from source 

### Prerequisites 

1. macOS: install [brew](https://brew.sh)

### Build from source

1. Add these lines to your bash profile:
  - `# QT`
    `export PATH="/usr/local/opt/qt@5/bin:$PATH"`
    `export LDFLAGS="-L/usr/local/opt/qt@5/lib"`
    `export CPPFLAGS="-I/usr/local/opt/qt@5/include"`
    `export PKG_CONFIG_PATH="/usr/local/opt/qt@5/lib/pkgconfig"`

2. Refresh/reload your bash profile

3. Go to root directory of this FinalHE, then `make clean`

4. `brew install libusb pkg-config qt@5 dyld-headers`

5. `cmake -DCMAKE_PREFIX_PATH=/usr/local/opt/qt@5 .`

6. `make`

7. Download FinalHE macOS pre-built [release is v1.5](https://github.com/soarqin/finalhe/releases/tag/v1.5)

8. Right click on FinalHE, then "Show Package Contents"

9. Replace the original FinalHE on Contents/MacOS with the one you just built `<SourceRoot/src/FinalHE>`

10. Run the FinalHE.app
