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
#include <stdlib.h>

void draw_tui(void) {
  int c = 0, x = 1, y = 1;
  ru_cls();
  while ((c = ru_getkey())) {
    switch (c) {
    case 'j':
      y++;
      break;
    case 'k':
      y--;
      break;
    case 'h':
      x--;
      break;
    case 'l':
      x++;
      break;
    }
    ru_locate(x, y);
  }
}
