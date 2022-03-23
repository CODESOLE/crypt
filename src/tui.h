/* tui.h
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

#ifndef _TUI_H_
#define _TUI_H_

#include "rogueutil/rogueutil.h"
#include <time.h>

enum option_stacks {
  FILE_CHOOSER = 0,
  MODE,
  OUTPUT,
  OUTPUT_FILE_NAMER,
  MAX_OPTION_STACK
};

void print_scr(enum option_stacks);

void draw_tui(void);

#endif /* _TUI_H_ */
