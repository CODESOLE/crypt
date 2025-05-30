# CRYPT
Simple hashing(SHA256) & encrypting/decrypting(AES128) commandline program

![](https://github.com/CODESOLE/crypt/blob/main/demo.gif)

# Build on Windows
 - Install [scoop](https://scoop.sh)
 - Install gcc, make and git : `scoop install make gcc git`
 - Open `Git-Bash` from start menu. Change directory to repo: `cd path/to/git/repo`.
 - Run make: `make`

# Build on Linux

You can install dependencies according to your package manager:
 - GCC
 - GNU Make

Then just run `make` from root directory

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
