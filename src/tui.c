/* tui.c
 *
 * Copyright 2022 CODESOLE
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include "tui.h"
#include <dirent.h>
#include <math.h>
#include <stdlib.h>
#include <sys/stat.h>

static int option_pos = FILE_CHOOSER;
#define MAX_FILE 8192
#define MAX_FILE_NAME 8192
static char files[MAX_FILE][MAX_FILE_NAME];
static unsigned int file_count;
static unsigned short file_idx;

void print_scr(enum option_stacks os) {
  ru_color_print(
      os == FILE_CHOOSER ? RU_RED : RU_WHITE, RU_BLACK,
      "╔═ Choose File ══════════════════════════════════════════════╗\n"
      "‖                                                            ‖\n"
      "╚════════════════════════════════════════════════════════════╝\n");
  ru_locate(40, 1);
  ru_color_print(os == FILE_CHOOSER ? RU_RED : RU_WHITE, RU_BLACK, " %d/%d ",
                 file_idx, file_count);
  ru_locate(2, 2);
  file_idx ? ru_color_print(os == FILE_CHOOSER ? RU_RED : RU_WHITE, RU_BLACK,
                            " %s ", files[file_idx - 1])
           : ru_print_xy(2, 2, "<NONE>");

  ru_locate(1, 4);
  ru_color_print(
      os == MODE ? RU_RED : RU_WHITE, RU_BLACK,
      "╔═ Choose Encrypt Mode ══════════════════════════════════════╗\n"
      "‖                                                            ‖\n"
      "╚════════════════════════════════════════════════════════════╝\n");

  ru_color_print(
      os == OUTPUT ? RU_RED : RU_WHITE, RU_BLACK,
      "╔═ Choose Output Mode ═══════════════════════════════════════╗\n"
      "‖                                                            ‖\n"
      "╚════════════════════════════════════════════════════════════╝\n");

  // if (/* OUTPUT != STDOUT */);
  ru_color_print(
      os == OUTPUT_FILE_NAMER ? RU_RED : RU_WHITE, RU_BLACK,
      "╔═ Choose Output File Name ══════════════════════════════════╗\n"
      "‖                                                            ‖\n"
      "╚════════════════════════════════════════════════════════════╝\n");
}

void draw_tui(void) {
  DIR *dir = NULL;
  struct dirent *de = NULL;
  struct stat path_stat;
  if ((dir = opendir(".")) == NULL) {
    perror("error opening directory");
    exit(EXIT_FAILURE);
  }
  while ((de = readdir(dir)) != NULL) {
    if (de->d_name[0] == '.')
      continue;
    stat(de->d_name, &path_stat);
    if (S_ISREG(path_stat.st_mode)) {
      strncpy(files[file_count], de->d_name, strlen(de->d_name) + 1);
      files[file_count][strlen(de->d_name)] = '\0';
      file_count++;
    }
  }
  closedir(dir);

  int c = 0;
  ru_hide_cursor();
  ru_cls();
  ru_locate(1, 1);
  if (file_count)
    file_idx++;
  do {
    switch (c) {
    case 'k':
      option_pos--;
      option_pos =
          option_pos < FILE_CHOOSER ? MAX_OPTION_STACK - 1 : option_pos;
      break;
    case 'j':
      option_pos++;
      option_pos = option_pos >= MAX_OPTION_STACK ? 0 : option_pos;
      break;
    case 'h':
      if (file_count && option_pos == FILE_CHOOSER) {
        file_idx--;
        file_idx = file_idx < 1 ? 1 : file_idx;
      }
      break;
    case 'l':
      if (file_count && option_pos == FILE_CHOOSER) {
        file_idx++;
        file_idx = file_idx > file_count ? file_count : file_idx;
      }
      break;
    default:
      break;
    }
    ru_cls();
    ru_locate(1, 1);
    print_scr(option_pos);
  } while ((c = ru_getkey()) != RU_KEY_ESCAPE);
  ru_cls();
  ru_show_cursor();
}
