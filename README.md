# CRYPT
Simple hashing(SHA256) & encrypting/decrypting(AES128) commandline program

![](https://github.com/CODESOLE/crypt/blob/main/demo.gif)

# Build on Windows
 - Install [MSYS2](https://msys2.org) (Leave installation directory to `C:\msys64`)
 - Open `MSYS2 MSYS` shell from start menu (**NOT `MSYS2 MINGW or others`**)
 - Change directory to the root of this folder and then run `./install_deps_for_windowsmsys2_and_arch_linux.sh` from `MSYS2 MSYS` shell ( it will install necessary dependencies)
 - Then close `MSYS2 MSYS` shell and open `MSYS2 MINGW64` shell, change directory to the root of this folder, run `./build.sh` this will create a `build` directory and write build files and compiled binary there (You can run `./build.sh r` to compile with release mode otherwise it will compile default to debug mode)

# Build on Linux
For Archlinux and its derivatives (which uses pacman package manager) you can run `./install_deps_for_windowsmsys2_and_arch_linux.sh` (if needed run with sudo), then run `./build.sh` (You can run `./build.sh r` to compile with release mode otherwise it will compile default to debug mode)

For other linux distributions you can install dependencies according to your package manager:
 - GCC
 - GNU Make
 - Premake

Then just run `./build.sh` from root directory (No need for `./install_deps_for_windowsmsys2_and_arch_linux.sh` other than arch linux like distributions)

# Usage
```
Usage: crypt [OPTION...] [FILE...]
simple hashing and encrypting/decrypting program.

-v       print version
-t       run in TUI mode
-h       print help
-m <arg> specify mode which can be either sha256 or aes128
-o <arg> specify out file to write aes encrypted/decrypted file, if not specified, it will print to stdout
-f <arg> specify file to encrypt/decrypt or to hash
```
