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
#include "libbsd_string/string.h"
#include <dirent.h>
#include <math.h>
#include <stdlib.h>
#include <sys/stat.h>

static int option_pos = FILE_CHOOSER;
#define MAX_FILE 8192
#define MAX_FILE_NAME 59
static char files[MAX_FILE][MAX_FILE_NAME];
static unsigned int file_count;
static unsigned short file_idx;

#define AES 1
#define SHA 0
static _Bool aes_or_sha;
#define FILE 0
#define STDOUT 1
static _Bool stdout_or_file;

void print_scr(enum option_stacks os) {
  ru_color_print(
      os == FILE_CHOOSER ? RU_RED : RU_WHITE, RU_BLACK,
#ifdef _WIN32
      "+- Choose File ----------------------------------------------+\n"
      "|                                                            |\n"
      "+------------------------------------------------------------+\n"
#else
      "╔═ Choose File ══════════════════════════════════════════════╗\n"
      "‖                                                            ‖\n"
      "╚════════════════════════════════════════════════════════════╝\n"
#endif
  );
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
#ifdef _WIN32
      "+- Choose Encrypt Mode --------------------------------------+\n"
      "|                                                            |\n"
      "+------------------------------------------------------------+\n"
#else
      "╔═ Choose Encrypt Mode ══════════════════════════════════════╗\n"
      "‖                                                            ‖\n"
      "╚════════════════════════════════════════════════════════════╝\n"
#endif
  );
  ru_locate(10, 5);
  ru_color_print(aes_or_sha == AES ? RU_RED : RU_WHITE, RU_BLACK, "<AES>");
  ru_locate(48, 5);
  ru_color_print(aes_or_sha == SHA ? RU_RED : RU_WHITE, RU_BLACK, "<SHA>");

  if (aes_or_sha == AES) {
    ru_locate(1, 7);
    ru_color_print(
        os == OUTPUT ? RU_RED : RU_WHITE, RU_BLACK,
#ifdef _WIN32
        "+- Choose Output Mode ---------------------------------------+\n"
        "|                                                            |\n"
        "+------------------------------------------------------------+\n"
#else
        "╔═ Choose Output Mode ═══════════════════════════════════════╗\n"
        "‖                                                            ‖\n"
        "╚════════════════════════════════════════════════════════════╝\n"
#endif
    );
    ru_locate(10, 8);
    ru_color_print(stdout_or_file == STDOUT ? RU_RED : RU_WHITE, RU_BLACK,
                   "<STDOUT>");
    ru_locate(48, 8);
    ru_color_print(stdout_or_file == FILE ? RU_RED : RU_WHITE, RU_BLACK,
                   "<FILE>");
  }
}

struct tui_options draw_tui(void) {
  struct tui_options tui_opt = {0};
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
      strlcpy(files[file_count], de->d_name, MAX_FILE_NAME);
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
      if (option_pos == MODE)
        aes_or_sha = !aes_or_sha;
      if (option_pos == OUTPUT)
        stdout_or_file = !stdout_or_file;
      break;
    case 'l':
      if (file_count && option_pos == FILE_CHOOSER) {
        file_idx++;
        file_idx = file_idx > file_count ? file_count : file_idx;
      }
      if (option_pos == MODE)
        aes_or_sha = !aes_or_sha;
      if (option_pos == OUTPUT)
        stdout_or_file = !stdout_or_file;
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
  strlcpy(tui_opt.filename, files[file_idx - 1], sizeof(tui_opt.filename));
  tui_opt.mode = aes_or_sha;
  tui_opt.output_type = stdout_or_file;

  return tui_opt;
}
