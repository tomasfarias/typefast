#include <ncurses.h>
#include <string.h>
#include <stdlib.h>
#include "helpers.h"

void append (char* s, int c) {
    int len = strlen(s);
    s[len] = c;
    s[len + 1] = '\0';
}

int char_in_pos (char* s, int c, int pos) {
    if (s[pos] == c) {
        return 0;
    }
    return -1;
}

int move_cursor_vertical (int y, int x, int maxpos, int curpos) {
    int ch;

    while (ch = getch()) {
        switch (ch) {
            case ERR:
            case KEY_EXIT:
            case 27:
            default: /* Something went wrong */
                clear();
                refresh();
                endwin();
                exit(0);
            case KEY_DOWN:
                if (curpos < maxpos) {
                    ++curpos;
                    move(++y, x);
                    refresh();
                }
                break;
            case KEY_UP:
                if (curpos > 0) {
                    --curpos;
                    move(--y, x);
                    refresh();
                }
                break;
            case KEY_ENTER:
            case 10:
                return curpos;
        }
    }
}
