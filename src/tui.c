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
#include <math.h>
#include <stdlib.h>

static int option_pos = FILE_CHOOSER;

void print_scr(enum option_stacks os) {
  ru_color_print(
      os == FILE_CHOOSER ? RU_RED : RU_WHITE, RU_BLACK,
      "╔═ Choose File ══════════════════════════════════════════════╗\n"
      "‖                                                            ‖\n"
      "╚════════════════════════════════════════════════════════════╝\n");
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
  int c = 0;
  ru_hide_cursor();
  ru_cls();
  ru_locate(1, 1);
  while ((c = ru_getkey()) != RU_KEY_ESCAPE) {
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
    default:
      break;
    }
    ru_cls();
    ru_locate(1, 1);
    print_scr(option_pos);
  }
  ru_show_cursor();
}
