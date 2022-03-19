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
//--------------------------------------------------------
// FUNCTION moving_and_sleeping
//--------------------------------------------------------
void moving_and_sleeping()
{
  int row = 5;
  int col = 0;

  curs_set(0);

  for (char c = 65; c <= 90; c++) {
    move(row++, col++);
    addch(c);
    refresh();
    napms(100);
  }

  row = 5;
  col = 3;

  for (char c = 97; c <= 122; c++) {
    mvaddch(row++, col++, c);
    refresh();
    napms(100);
  }

  curs_set(1);

  addch('\n');
}
//--------------------------------------------------------
// FUNCTION printing
//--------------------------------------------------------
void printing()
{
  addstr("This was printed using addstr\n\n");
  refresh();

  addstr("The following letter was printed using addch:- ");
  addch('a');
  refresh();

  printw("\n\nThese numbers were printed using printw\n%d\n%f\n", 123, 456.789);
  refresh();
}
//--------------------------------------------------------
// FUNCTION colouring
//--------------------------------------------------------
void coloring()
{
  if (has_colors()) {
    if (start_color() == OK) {
      for (size_t i = 0; i < 255; ++i)
        for (size_t j = 0; j < 255; ++j)
          init_pair(i +j, i, j);

      for (size_t i = 0; i < 65535; ++i) {
        attrset(COLOR_PAIR(i));
        addch('@');
        refresh();
        attroff(COLOR_PAIR(i));
      }
    } else {
      addstr("Cannot start colours\n");
      refresh();
    }
  } else {
    addstr("Not colour capable\n");
    refresh();
  }
}
void
draw_tui(void)
{
  initscr();

  coloring();

  endwin();
}
