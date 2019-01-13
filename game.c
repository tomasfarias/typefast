#include <ncurses.h>
#include "screens.h"
#include "helpers.h"

int main (void) {
    char curr[MAX_LEN];
    FILE *file;
    int correct, maxlines, maxcols;
    struct Result result;
    char filename[] = "theoldmanandthesea.txt";
    int max_chars = MAX_LEN;

    /* Initialize curses */
    initscr();
    cbreak();
    keypad(stdscr, TRUE);
    noecho();

    maxlines = LINES - 1;
    maxcols = COLS - 1;

    /* Game loop */
    while (1) {
        title_screen(maxlines, maxcols);
        result = game_screen(maxlines, maxcols, filename, MAX_LEN);
        end_screen(maxlines, maxcols, result);
    }

    endwin();
    return 0;
}
