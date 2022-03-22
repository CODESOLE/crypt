#include "rogueutil.h"

/* Constant strings for ANSI colors ans seqiences */
static const char *RU_ANSI_CLS                = "\033[2J\033[3J";
static const char *RU_ANSI_CONSOLE_TITLE_PRE  = "\033]0;";
static const char *RU_ANSI_CONSOLE_TITLE_POST = "\007";
static const char *RU_ANSI_ATTRIBUTE_RESET    = "\033[0m";
static const char *RU_ANSI_CURSOR_HIDE        = "\033[?25l";
static const char *RU_ANSI_CURSOR_SHOW        = "\033[?25h";
static const char *RU_ANSI_CURSOR_HOME        = "\033[H";
static const char *RU_ANSI_BLACK              = "\033[22;30m";
static const char *RU_ANSI_RED                = "\033[22;31m";
static const char *RU_ANSI_GREEN              = "\033[22;32m";
static const char *RU_ANSI_BROWN              = "\033[22;33m";
static const char *RU_ANSI_BLUE               = "\033[22;34m";
static const char *RU_ANSI_MAGENTA            = "\033[22;35m";
static const char *RU_ANSI_CYAN               = "\033[22;36m";
static const char *RU_ANSI_GREY               = "\033[22;37m";
static const char *RU_ANSI_DARKGREY           = "\033[01;30m";
static const char *RU_ANSI_LIGHTRED           = "\033[01;31m";
static const char *RU_ANSI_LIGHTGREEN         = "\033[01;32m";
static const char *RU_ANSI_YELLOW             = "\033[01;33m";
static const char *RU_ANSI_LIGHTBLUE          = "\033[01;34m";
static const char *RU_ANSI_LIGHTMAGENTA       = "\033[01;35m";
static const char *RU_ANSI_LIGHTCYAN          = "\033[01;36m";
static const char *RU_ANSI_WHITE              = "\033[01;37m";
static const char *RU_ANSI_BACKGROUND_BLACK   = "\033[40m";
static const char *RU_ANSI_BACKGROUND_RED     = "\033[41m";
static const char *RU_ANSI_BACKGROUND_GREEN   = "\033[42m";
static const char *RU_ANSI_BACKGROUND_YELLOW  = "\033[43m";
static const char *RU_ANSI_BACKGROUND_BLUE    = "\033[44m";
static const char *RU_ANSI_BACKGROUND_MAGENTA = "\033[45m";
static const char *RU_ANSI_BACKGROUND_CYAN    = "\033[46m";
static const char *RU_ANSI_BACKGROUND_WHITE   = "\033[47m";
/* Remaining colors not supported as background colors */

#ifndef _WIN32
int
getch(void)
{
	struct termios oldt, newt;
	int ch;
	tcgetattr(STDIN_FILENO, &oldt);
	newt = oldt;
	newt.c_lflag &= ~(ICANON | ECHO);
	tcsetattr(STDIN_FILENO, TCSANOW, &newt);
	ch = getchar();
	tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
	return ch;
}

int
kbhit(void)
{
	static struct termios oldt, newt;
	int cnt = 0;
	tcgetattr(STDIN_FILENO, &oldt);
	newt = oldt;
	newt.c_lflag    &= ~(ICANON | ECHO);
	newt.c_iflag     = 0; /* input mode */
	newt.c_oflag     = 0; /* output mode */
	newt.c_cc[VMIN]  = 1; /* minimum time to wait */
	newt.c_cc[VTIME] = 1; /* minimum characters to wait for */
	tcsetattr(STDIN_FILENO, TCSANOW, &newt);
	ioctl(0, FIONREAD, &cnt); /* Read count */
	struct timeval tv;
	tv.tv_sec  = 0;
	tv.tv_usec = 100;
	select(STDIN_FILENO+1, NULL, NULL, NULL, &tv); /* A small time delay */
	tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
	return cnt; /* Return number of characters */
}

#endif /* _WIN32 */

#ifndef gotoxy

void
ru_gotoxy(int x, int y)
{
  ru_locate(x,y);
}
#endif /* gotoxy */

/**
 * @brief Printing wrapper independent of C/C++
 * @param st String to print
 * @see color_print()
 * @see colorPrintBG()
 */
static void
ru_print(const char *st)
{
  printf("%s", st);
}

int
ru_getkey(void)
{
#ifndef _WIN32
	int cnt = kbhit(); /* for ANSI escapes processing */
#endif
	int k = getch();
	switch(k) {
	case 0: {
		int kk;
		switch (kk = getch()) {
		case 71:
			return RU_KEY_NUMPAD7;
		case 72:
			return RU_KEY_NUMPAD8;
		case 73:
			return RU_KEY_NUMPAD9;
		case 75:
			return RU_KEY_NUMPAD4;
		case 77:
			return RU_KEY_NUMPAD6;
		case 79:
			return RU_KEY_NUMPAD1;
		case 80:
			return RU_KEY_NUMPAD2;
		case 81:
			return RU_KEY_NUMPAD3;
		case 82:
			return RU_KEY_NUMPAD0;
		case 83:
			return RU_KEY_NUMDEL;
		default:
			return kk-59+RU_KEY_F1; /* Function keys */
		}
	}
	case 224: {
		int kk;
		switch (kk = getch()) {
		case 71:
			return RU_KEY_HOME;
		case 72:
			return RU_KEY_UP;
		case 73:
			return RU_KEY_PGUP;
		case 75:
			return RU_KEY_LEFT;
		case 77:
			return RU_KEY_RIGHT;
		case 79:
			return RU_KEY_END;
		case 80:
			return RU_KEY_DOWN;
		case 81:
			return RU_KEY_PGDOWN;
		case 82:
			return RU_KEY_INSERT;
		case 83:
			return RU_KEY_DELETE;
		default:
			return kk-123+RU_KEY_F1; /* Function keys */
		}
	}
	case 13:
		return RU_KEY_ENTER;
#ifdef _WIN32
	case 27:
		return RU_KEY_ESCAPE;
#else /* _WIN32 */
	case 155: /* single-character CSI */
	case 27: {
		/* Process ANSI escape sequences */
		if (cnt >= 3 && getch() == '[') {
			switch (k = getch()) {
			case 'A':
				return RU_KEY_UP;
			case 'B':
				return RU_KEY_DOWN;
			case 'C':
				return RU_KEY_RIGHT;
			case 'D':
				return RU_KEY_LEFT;
			default:
				return RU_KEY_ESCAPE;
			}
		} else return RU_KEY_ESCAPE;
	}
#endif /* _WIN32 */
	default:
		return k;
	}
}

int
ru_nb_getch(void)
{
	if (kbhit()) return getch();
	else return 0;
}

const char *
ru_get_ansi_color(const int c)
{
	switch (c) {
	case RU_BLACK       :
		return RU_ANSI_BLACK;
	case RU_BLUE        :
		return RU_ANSI_BLUE; /* non-ANSI */
	case RU_GREEN       :
		return RU_ANSI_GREEN;
	case RU_CYAN        :
		return RU_ANSI_CYAN; /* non-ANSI */
	case RU_RED         :
		return RU_ANSI_RED; /* non-ANSI */
	case RU_MAGENTA     :
		return RU_ANSI_MAGENTA;
	case RU_BROWN       :
		return RU_ANSI_BROWN;
	case RU_GREY        :
		return RU_ANSI_GREY;
	case RU_DARKGREY    :
		return RU_ANSI_DARKGREY;
	case RU_LIGHTBLUE   :
		return RU_ANSI_LIGHTBLUE; /* non-ANSI */
	case RU_LIGHTGREEN  :
		return RU_ANSI_LIGHTGREEN;
	case RU_LIGHTCYAN   :
		return RU_ANSI_LIGHTCYAN; /* non-ANSI; */
	case RU_LIGHTRED    :
		return RU_ANSI_LIGHTRED; /* non-ANSI; */
	case RU_LIGHTMAGENTA:
		return RU_ANSI_LIGHTMAGENTA;
	case RU_YELLOW      :
		return RU_ANSI_YELLOW; /* non-ANSI */
	case RU_WHITE       :
		return RU_ANSI_WHITE;
	default:
		return "";
	}
}

const char *
ru_get_ansi_bg_color(const int c)
{
	switch (c) {
	case RU_BLACK  :
		return RU_ANSI_BACKGROUND_BLACK;
	case RU_BLUE   :
		return RU_ANSI_BACKGROUND_BLUE;
	case RU_GREEN  :
		return RU_ANSI_BACKGROUND_GREEN;
	case RU_CYAN   :
		return RU_ANSI_BACKGROUND_CYAN;
	case RU_RED    :
		return RU_ANSI_BACKGROUND_RED;
	case RU_MAGENTA:
		return RU_ANSI_BACKGROUND_MAGENTA;
	case RU_BROWN  :
		return RU_ANSI_BACKGROUND_YELLOW;
	case RU_GREY   :
		return RU_ANSI_BACKGROUND_WHITE;
	default:
		return "";
	}
}

void
ru_set_color(int c)
{
#if defined(_WIN32) && !defined(RUTIL_USE_ANSI)
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO csbi;

	GetConsoleScreenBufferInfo(hConsole, &csbi);

	SetConsoleTextAttribute(hConsole, (csbi.wAttributes & 0xFFF0) | (WORD)c); // Foreground colors take up the least significant byte
#else
	ru_print(ru_get_ansi_color(c));
#endif
}

void
ru_set_bg_color(int c)
{
#if defined(_WIN32) && !defined(RUTIL_USE_ANSI)
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO csbi;

	GetConsoleScreenBufferInfo(hConsole, &csbi);

	SetConsoleTextAttribute(hConsole, (csbi.wAttributes & 0xFF0F) | (((WORD)c) << 4)); // Background colors take up the second-least significant byte
#else
	ru_print(ru_get_ansi_bg_color(c));
#endif
}

int
ru_save_default_color(void)
{
#if defined(_WIN32) && !defined(RUTIL_USE_ANSI)
	static char initialized = 0; /* bool */
	static WORD attributes;

	if (!initialized) {
		CONSOLE_SCREEN_BUFFER_INFO csbi;
		GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
		attributes = csbi.wAttributes;
		initialized = 1;
	}
	return (int)attributes;
#else
	return -1;
#endif
}

void
ru_reset_color(void)
{
#if defined(_WIN32) && !defined(RUTIL_USE_ANSI)
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), (WORD)ru_save_default_color());
#else
	ru_print(RU_ANSI_ATTRIBUTE_RESET);
#endif
}

void
ru_cls(void)
{
#if defined(_WIN32) && !defined(RUTIL_USE_ANSI)
	/* Based on https://msdn.microsoft.com/en-us/library/windows/desktop/ms682022%28v=vs.85%29.aspx */
	const HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	const COORD coordScreen = {0, 0};
	DWORD cCharsWritten;
	CONSOLE_SCREEN_BUFFER_INFO csbi;

	GetConsoleScreenBufferInfo(hConsole, &csbi);
	const DWORD dwConSize = csbi.dwSize.X * csbi.dwSize.Y;
	FillConsoleOutputCharacter(hConsole, (TCHAR)' ', dwConSize, coordScreen, &cCharsWritten);

	GetConsoleScreenBufferInfo(hConsole, &csbi);
	FillConsoleOutputAttribute(hConsole, csbi.wAttributes, dwConSize, coordScreen, &cCharsWritten);

	SetConsoleCursorPosition(hConsole, coordScreen);
#else
	ru_print(RU_ANSI_CLS);
	ru_print(RU_ANSI_CURSOR_HOME);
#endif
}

void
ru_locate(int x, int y)
{
#if defined(_WIN32) && !defined(RUTIL_USE_ANSI)
	COORD coord;
	coord.X = (SHORT)(x - 1);
	coord.Y = (SHORT)(y - 1); /* Windows uses 0-based coordinates */
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
#else /* _WIN32 || USE_ANSI */
	char buf[32];
	sprintf(buf, "\033[%d;%df", y, x);
	ru_print(buf);
#endif /* _WIN32 || USE_ANSI */
}

void
ru_set_string(const char *str)
{
	unsigned int len = (unsigned int)strlen(str);
#if defined(_WIN32) && !defined(RUTIL_USE_ANSI)
	HANDLE hConsoleOutput = GetStdHandle(STD_OUTPUT_HANDLE);
	DWORD numberOfCharsWritten;
	CONSOLE_SCREEN_BUFFER_INFO csbi;

	GetConsoleScreenBufferInfo(hConsoleOutput, &csbi);

#ifdef UNICODE
	WriteConsoleOutputCharacterA(hConsoleOutput, str,
                                     len, csbi.dwCursorPosition,
                                     &numberOfCharsWritten);
#else
	WriteConsoleOutputCharacter(hConsoleOutput, str,
                                    len, csbi.dwCursorPosition,
                                    &numberOfCharsWritten);
#endif /* UNICODE */

#else /* _WIN32 || USE_ANSI */
	ru_print(str);
	char buf[3 + 20 + 1]; /* 20 = max length of 64-bit
                                 * unsigned int when printed as dec */
	sprintf(buf, "\033[%uD", len);
	ru_print(buf);
#endif /* _WIN32 || USE_ANSI */
}

void
ru_set_char(char ch)
{
	const char buf[] = {ch, 0};
	ru_set_string(buf);
}

void
ru_set_cursor_visibility(char visible)
{
#if defined(_WIN32) && !defined(RUTIL_USE_ANSI)
	HANDLE hConsoleOutput = GetStdHandle( STD_OUTPUT_HANDLE );
	CONSOLE_CURSOR_INFO structCursorInfo;
	GetConsoleCursorInfo( hConsoleOutput, &structCursorInfo ); // Get current cursor size
	structCursorInfo.bVisible = (visible ? TRUE : FALSE);
	SetConsoleCursorInfo( hConsoleOutput, &structCursorInfo );
#else /* _WIN32 || USE_ANSI */
	ru_print((visible ? RU_ANSI_CURSOR_SHOW : RU_ANSI_CURSOR_HIDE));
#endif /* _WIN32 || USE_ANSI */
}

void
ru_hide_cursor(void)
{
	ru_set_cursor_visibility(0);
}

void
ru_show_cursor(void)
{
	ru_set_cursor_visibility(1);
}

int
ru_total_rows(void)
{
#ifdef _WIN32
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	if (!GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi))
		return -1;
	else
		return csbi.srWindow.Bottom - csbi.srWindow.Top + 1; // Window height
#else
#ifdef TIOCGSIZE
	struct ttysize ts;
	ioctl(STDIN_FILENO, TIOCGSIZE, &ts);
	return ts.ts_lines;
#elif defined(TIOCGWINSZ)
	struct winsize ts;
	ioctl(STDIN_FILENO, TIOCGWINSZ, &ts);
	return ts.ws_row;
#else /* TIOCGSIZE */
	return -1;
#endif /* TIOCGSIZE */
#endif /* _WIN32 */
}

int
ru_total_cols(void)
{
#ifdef _WIN32
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	if (!GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi))
		return -1;
	else
		return csbi.srWindow.Right - csbi.srWindow.Left + 1; // Window width
#else
#ifdef TIOCGSIZE
	struct ttysize ts;
	ioctl(STDIN_FILENO, TIOCGSIZE, &ts);
	return ts.ts_cols;
#elif defined(TIOCGWINSZ)
	struct winsize ts;
	ioctl(STDIN_FILENO, TIOCGWINSZ, &ts);
	return ts.ws_col;
#else /* TIOCGSIZE */
	return -1;
#endif /* TIOCGSIZE */
#endif /* _WIN32 */
}

void
ru_anykey(char *msg)
{
	if (msg)
		ru_print(msg);
	getch();
}

void
ru_set_console_title(char *title)
{
	const char * true_title = title;
#if defined(_WIN32) && !defined(RUTIL_USE_ANSI)
	SetConsoleTitleA(true_title);
#else
	ru_print(RU_ANSI_CONSOLE_TITLE_PRE);
	ru_print(true_title);
	ru_print(RU_ANSI_CONSOLE_TITLE_POST);
#endif /* defined(_WIN32) && !defined(RUTIL_USE_ANSI) */
}

void
ru_color_print(ru_color_code color, ru_color_code bgcolor, const char *fmt, ...)
{
	va_list args;
	va_start(args, fmt);

	if (color >= 0)
        	ru_set_color(color);

	if (bgcolor >= 0)
		ru_set_bg_color(bgcolor);

        vprintf(fmt, args);
	va_end(args);

	ru_reset_color();
}

char*
ru_get_username(void)
{
#ifdef _WIN32
        /* TODO: Find a better way that doesn't use a static var. */
        static char ret[UNLEN + 1];
        DWORD len = UNLEN + 1;
        if (GetUserNameA(ret, &len))
                return ret;
        return NULL;
#else /* _WIN32 */
	struct passwd *pw = getpwuid(getuid());
	if (pw) {
		return pw->pw_name;
	} else {
		return NULL;
	}
#endif
}

void
ru_print_xy(int x, int y, char *msg)
{
  ru_locate(x, y);
  ru_print(msg);
}
