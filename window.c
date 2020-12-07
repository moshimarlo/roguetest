#include "window.h"
#define winHeight 3
#define winWidth 5
#define winx1 0
#define winy1 0


WINDOW *debug_win;

WINDOW *initWindow() {
    debug_win = newwin(winHeight, winWidth, winx1, winy1);
    wrefresh(debug_win);
    return debug_win;
}
