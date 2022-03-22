/**
 * @section LICENSE
 * Copyright 2020 Sergei Akhmatdinov
 *
 * Licensed under the Apache License, Version 2.0 (the "License"); * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied *.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/**
 * @file rogueutil.h
 * @brief Cross-platform C/C++ utility for creating text-based user interfaces
 *
 * @section Description
 * Rogueutil provides some useful functions for creating TUIs and console-based
 * games, such as color printing and clearing the console.
 * It can be used both as a C or C++ header file and it aims at being
 * cross-platform between Windows and *nix.
 *
 */

#ifndef RUTIL_H
#define RUTIL_H

#include <stdio.h> /* for getch() / printf() */
#include <string.h> /* for strlen() */
#include <stdarg.h> /* for color_print() */

void ru_locate(int x, int y); /* Forward declare for C to avoid warnings */

#ifdef _WIN32
	#include <windows.h>  /* for WinAPI and Sleep() */
	#define _NO_OLDNAMES  /* for MinGW compatibility */
	#include <conio.h>    /* for getch() and kbhit() */
	#include <lmcons.h>   /* for getUsername()      */
	#define getch _getch
	#define kbhit _kbhit
#else
	#include <termios.h> /* for getch() and kbhit() */
	#include <unistd.h> /* for getch(), kbhit() and getuid() */
	#include <time.h>   /* for nanosleep() */
	#include <sys/ioctl.h> /* for getkey() */
	#include <sys/types.h> /* for kbhit() */
	#include <sys/time.h> /* for kbhit() */
	#include <pwd.h> /* for getpwuid() */
#endif

/* Functions covered by Window's conio.h */
#ifndef _WIN32

/**
 * @brief Get a charater without waiting on a Return
 * @details Windows has this functionality in conio.h
 * @return The character
 */
int
getch(void);

/**
 * @brief Determines if a button was pressed.
 * @details Windows has this in conio.h
 * @return Number of characters read
 */
int
kbhit(void);

#endif /* _WIN32 */


#ifndef gotoxy
/**
 * @brief Sets the cursor to the specified x and y position.
 * @details Windows has this functionality in conio.h
 * @see locate()
 */
void
ru_gotoxy(int x, int y);
#endif /* gotoxy */

/**
 * @brief Provides easy color codes with similar numbers to QBasic
 */
typedef enum ru_color_code {
	RU_BLACK,
	RU_BLUE,
	RU_GREEN,
	RU_CYAN,
	RU_RED,
	RU_MAGENTA,
	RU_BROWN,
	RU_GREY,
	RU_DARKGREY,
	RU_LIGHTBLUE,
	RU_LIGHTGREEN,
	RU_LIGHTCYAN,
	RU_LIGHTRED,
	RU_LIGHTMAGENTA,
	RU_YELLOW,
	RU_WHITE
} ru_color_code;

/**
 * @brief Provides keycodes for special keys
 */
typedef enum ru_key_code {
	RU_KEY_ESCAPE  = 0,
	RU_KEY_ENTER   = 1,
	RU_KEY_SPACE   = 32,

	RU_KEY_INSERT  = 2,
	RU_KEY_HOME    = 3,
	RU_KEY_PGUP    = 4,
	RU_KEY_DELETE  = 5,
	RU_KEY_END     = 6,
	RU_KEY_PGDOWN  = 7,

	RU_KEY_UP      = 14,
	RU_KEY_DOWN    = 15,
	RU_KEY_LEFT    = 16,
	RU_KEY_RIGHT   = 17,

	RU_KEY_F1      = 18,
	RU_KEY_F2      = 19,
	RU_KEY_F3      = 20,
	RU_KEY_F4      = 21,
	RU_KEY_F5      = 22,
	RU_KEY_F6      = 23,
	RU_KEY_F7      = 24,
	RU_KEY_F8      = 25,
	RU_KEY_F9      = 26,
	RU_KEY_F10     = 27,
	RU_KEY_F11     = 28,
	RU_KEY_F12     = 29,

	RU_KEY_NUMDEL  = 30,
	RU_KEY_NUMPAD0 = 31,
	RU_KEY_NUMPAD1 = 127,
	RU_KEY_NUMPAD2 = 128,
	RU_KEY_NUMPAD3 = 129,
	RU_KEY_NUMPAD4 = 130,
	RU_KEY_NUMPAD5 = 131,
	RU_KEY_NUMPAD6 = 132,
	RU_KEY_NUMPAD7 = 133,
	RU_KEY_NUMPAD8 = 134,
	RU_KEY_NUMPAD9 = 135,
} ru_key_code;

/**
 * @brief Reads a key press (blocking)
 * @details At the moment, only Arrows, ESC, Enter and Space are working
 * @return Key code that was read
 */
int
ru_getkey(void);

/**
 * @brief Non-blocking version of getch()
 * @return The character pressed or 0 if no key was pressed.
 * @see getch()
 */
int
ru_nb_getch(void);

/**
 * @brief Returns ANSI color escape sequence for specified number
 * @param c Number 0-15 corresponding to the color code
 * @see color_code
 */
const char *
ru_get_ansi_color(const int c);

/**
 * @brief Returns the ANSI background color escape sequence
 * @param c Number 0-15 corresponding to the color code
 * @see color_code
 */
const char *
ru_get_ansi_bg_color(const int c);

/**
 * @brief Changes color as specified by a number
 * @param c Number 0-15 corresponding to a color code
 * @see color_code
 */
void
ru_set_color(int c);

/**
 * @brief Changes the background color as specified by a number
 * @param c Number 0-15 corresponding to a color code
 * @see color_code
 */
void
ru_set_bg_color(int c);

/**
 * @brief Saves the color to use in resetColor() on Windows
 * @detail Returns -1 if not on Windows or if RUTIL_USE_ANSI is defined
 */
int
ru_save_default_color(void);

/**
 * @brief Resets the color to one set by saveDefaultColor()
 * @see color_code
 * @see setColor()
 * @see saveDefaultColor()
 */
void
ru_reset_color(void);

/**
 * @brief Clears screen, resets all attributes and moves cursor home.
 */
void
ru_cls(void);

/**
 * @brief Sets the cursor position to one defined by x and y.
 */
void
ru_locate(int x, int y);

/**
 * @brief Prints the supplied string without advancing the cursor
 */
void
ru_set_string(const char *str);

/**
 * @brief Sets the character at the cursor without advancing the cursor
 */
void
ru_set_char(char ch);

/**
 * @brief Shows/hides the cursor.
 * @param visible 0 to hide the cursor, anything else to show the cursor
 */
void
ru_set_cursor_visibility(char visible);

/**
 * @brief Hides the cursor
 * @see setCursorVisibility()
 */
void
ru_hide_cursor(void);

/**
 * @brief Shows the cursor
 * @see setCursorVisibility()
 */
void
ru_show_cursor(void);

/**
 * @brief Returns the number of rows in the terminal window or -1 on error.
 */
int
ru_total_rows(void);

/**
 * @brief Returns the number of columns in the terminal or -1 on error.
 */
int
ru_total_cols(void);

/**
 * @brief Waits until a key is pressed.
 * @param msg The message to display or NULL. Optional in C++.
 */
void
ru_anykey(char *msg);

/**
 * @brief Sets the console title given a string.
 */
void
ru_set_console_title(char *title);

/**
 * @brief Prints a message in a given color.
 * @param fmt printf-style formatted string to print in C or a list of objects in C++
 * @param color Foreground color to be used, use -1 to use the currently set foreground color
 * @param bgcolor Background color to be used, use -1 to use the currently set background color
 * @see color_code
 */
void
ru_color_print(ru_color_code color, ru_color_code bgcolor, const char *fmt, ...);

/**
 * @brief Returns the username of the user running the program.
 */
char*
ru_get_username(void);

/**
 * @brief Print a message at a position given by x and y.
 * @see locate()
 * @see rutil_print()
 */
void
ru_print_xy(int x, int y, char *msg);

#endif
