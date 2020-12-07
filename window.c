#include "window.h"
#include "string.h"

#define winHeight 3
#define winWidth 20 
#define winx1 0
#define winy1 0


WINDOW *debug_win;
WINDOW *game_win;


WINDOW *initWindow() {
    debug_win = newwin(winHeight, winWidth, winx1, winy1);
    wrefresh(debug_win);
    return debug_win;
}

void printToBuffer(char* buffer, char* input) {
    strcpy(buffer, input);
}

void printBuffer(char* buffer, WINDOW *window) {
    mvwprintw(window, 0, 0, buffer);
}
