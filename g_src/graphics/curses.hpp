#ifndef DF_GSRC_GRAPHICS_CURSES_HPP
#define DF_GSRC_GRAPHICS_CURSES_HPP

extern "C" {
#include "GL/glew.h"
#if defined(__unix__) || defined(__APPLE__)
#ifdef __APPLE__
#include "ncursesw/curses.h"
#else
#include <ncurses.h>
#endif
#undef COLOR_BLUE
#undef COLOR_CYAN
#undef COLOR_RED
#undef COLOR_YELLOW
#include <dlfcn.h>
#endif
}

#if defined(__unix__) || defined(__APPLE__)
extern "C" {
void init_curses();
extern WINDOW** stdscr_p;
};
#endif

#endif // DF_GSRC_GRAPHICS_CURSES_HPP
