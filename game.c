#include <stdio.h>
#include <string.h>
#include <ncurses.h>
#include <stdlib.h>

#define MAX_LEN 256

int option;
int correct;

void append(char* s, int c);
int char_in_pos(char* s, int c, int pos);
int end_screen(int lines, int cols, int correct, int size_words);
int title_screen(int lines, int cols);
int game_screen(int lines, int cols, char (*words)[MAX_LEN], int size_words, int lives);
int move_cursor_vertical(int y, int x, int maxpos, int curpos);

int main (void) {

    char curr[MAX_LEN];
    char words[4][MAX_LEN] = {
        "jotaro",
        "aloha",
        "miau",
        "ahre"
    };
    int correct, maxlines, maxcols, lives;
    size_t size_words = sizeof(words) / sizeof(words[0]);

    /* initialize curses */
    initscr();
    cbreak();
    keypad(stdscr, TRUE);
    noecho();

    maxlines = LINES - 1;
    maxcols = COLS - 1;

    while (1) {
        lives = 3;

        title_screen(maxlines, maxcols);
        correct = game_screen(maxlines, maxcols, words, size_words, lives);
        end_screen(maxlines, maxcols, correct, size_words);
    }

    endwin();
    return 0;
}

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

int title_screen (int lines, int cols) {
    int pos, y, x;
    int maxpos = 1;

    clear();
    refresh();

    mvprintw(lines / 2, cols / 2, "Typefast!");
    mvprintw(lines / 2 + 1, cols / 2, "Play");
    mvprintw(lines / 2 + 2, cols / 2, "Exit");
    getyx(stdscr, y, x);
    move(y - 1, cols / 2);
    refresh();

    pos = move_cursor_vertical(y - 1, cols / 2, maxpos, 0);

    switch (pos) {
        case 0:
            return 0;
        case 1:
        default:
            clear();
            refresh();
            endwin();
            exit(0);
    }
}


int end_screen (int lines, int cols, int correct, int size_words) {
    int y, x;
    int pos = 0;
    int maxpos = 1;
    char msg[20] = "You win!";

    if (correct < size_words) {
        strncpy(msg, "You lose!", 20);
    }

    clear();
    refresh();

    mvprintw(lines / 2, cols / 2, "%s", msg);
    mvprintw(lines / 2 + 1, cols / 2, "Correct words: %d/%d", correct, size_words);
    mvprintw(lines / 2 + 2, cols / 2, "Return to title");
    mvprintw(lines / 2 + 3, cols / 2, "Exit");
    getyx(stdscr, y, x);
    move(y - 1, cols / 2);
    refresh();

    pos = move_cursor_vertical(y - 1, cols / 2, maxpos, 0);
    switch (pos) {
        case 0:
            return 0;
        case 1:
        default:
            clear();
            refresh();
            endwin();
            exit(0);
    }
}

int game_screen (int lines, int cols, char (*words)[MAX_LEN], int size_words, int lives) {
    char curr[MAX_LEN];
    int ch, chpos, count;

    for (count = 0; count < size_words; ++count) {
        clear();
        refresh();

        chpos = 0;
        memset(curr, 0, sizeof(curr)); /* Reset current attemp */

        mvprintw(lines / 2, cols / 2, "%s", words[count]);
        mvprintw(0, cols, "Lives: %d", lives);
        refresh();

        while (lives > 0) {
            ch = getch();

            if (ch == ERR || ch == KEY_EXIT || ch == 27) {
                /* For now, we end everything */
                endwin();
                return 0;
            } else if (char_in_pos(words[count], ch, chpos) == 0) {
                /* Correct char */
                ++chpos;
                append(curr, ch);
                mvprintw(lines, cols / 2, curr);

                if (strcmp(curr, words[count]) == 0) {
                    break;
                }

            } else {
                /* Incorrect char */
                --lives;
                if (lives == 0) {
                    break;
                }
                mvprintw(lines, cols / 2, curr);
            }
            refresh();
        }

        if (lives != 0) {
            ++correct;
        }
    }

    return correct;
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
