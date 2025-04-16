# CRYPT
Simple hashing(SHA256) & encrypting/decrypting(AES128) commandline program

![](https://github.com/CODESOLE/crypt/blob/main/demo.gif)

# Build on Windows
 - Install [scoop](https://scoop.sh)
 - Install gcc, make and git : `scoop install make gcc git`
 - Open `Git-Bash` from start menu. Change directory to repo: `cd path/to/git/repo`.
 - Run make: `make`

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
