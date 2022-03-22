#!/bin/sh

if [ -z "${MSYSTEM}" ] && [ "$(uname)" = "Linux" ] && [ -x "$(command -v pacman)" ]; then
  echo LINUX_PACMAN
  pacman -S --needed --noconfirm base-devel premake
elif [ -n "${MSYSTEM}" ] && [ "${MSYSTEM}" = "MSYS" ]; then
  echo MSYS2_MSYS
  pacman -S --needed --noconfirm base-devel mingw-w64-x86_64-{toolchain,premake}
elif [ -n "${MSYSTEM}" ] && [ "${MSYSTEM}" != "MSYS" ]; then
  echo "You executed this in MSYS2 ${MSYSTEM} shell which is invalid MSYS2 shell. It should be executed in MSYS2 MSYS shell."
else
  echo "Unknown environment, this script file should be executed either in Linux with pacman package manager or in MSYS2_MSYS shell (NOT MSYS MINGW shell) on Windows."
fi
