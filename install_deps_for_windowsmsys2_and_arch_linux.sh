#!/bin/sh

if [ -z "${MSYSTEM}" ] && [ "$(uname)" = "Linux" ] && [ -x "$(command -v pacman)" ]; then
  echo LINUX_PACMAN
  pacman -S --needed --noconfirm base-devel premake ncurses
elif [ "${MSYSTEM}" = "MINGW64" ]; then
  echo MSYS2_MINGW64
  pacman -S --needed --noconfirm base-devel mingw-w64-x86_64-{toolchain,premake,ncurses}
else
  echo "Unknown environment, this script file should be executed either in Linux with pacman package manager or in MSYS2_MINGW64 shell on Windows."
fi
