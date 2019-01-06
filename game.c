#include <stdio.h>
#include <string.h>
#include <ncurses.h>
#include <stdlib.h>
#include <ctype.h>

#define MAX_LEN 256

struct Result {
    int correct;
    int lives;
};

void append(char* s, int c);
int char_in_pos(char* s, int c, int pos);
int end_screen(int lines, int cols, struct Result result);
int title_screen(int lines, int cols);
struct Result game_screen(int lines, int cols, char *filename);
int move_cursor_vertical(int y, int x, int maxpos, int curpos);
int lives_counter(int lives, int amount, int maxcols);

int main (void) {
    char curr[MAX_LEN];
    FILE *file;
    int correct, maxlines, maxcols;
    struct Result result;
    char filename[] = "theoldmanandthesea.txt";

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
        result = game_screen(maxlines, maxcols, filename);
        end_screen(maxlines, maxcols, result);
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
    int maxpos = 2;

    clear();
    refresh();

    mvprintw(lines / 2, cols / 2, "Typefast!");
    mvprintw(lines / 2 + 1, cols / 2, "Play");
    mvprintw(lines / 2 + 2, cols / 2, "File");
    mvprintw(lines / 2 + 3, cols / 2, "Exit");
    getyx(stdscr, y, x);
    move(y - 2, cols / 2);
    refresh();

    pos = move_cursor_vertical(y - 2, cols / 2, maxpos, 0);

    switch (pos) {
        case 0:
            return 0;
        case 1:
            return 0;
        case 2:
        default:
            clear();
            refresh();
            endwin();
            exit(0);
    }
}


int end_screen (int lines, int cols, struct Result result) {
    int y, x;
    int pos = 0;
    int maxpos = 1;
    char msg[20] = "You lose!";

    if (result.lives > 0) {
        strncpy(msg, "You win!", 20);
    }

    clear();
    refresh();

    mvprintw(lines / 2, cols / 2, "%s", msg);
    mvprintw(lines / 2 + 1, cols / 2, "Correct words: %d", result.correct);
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

struct Result game_screen (int lines, int cols, char *filename) {
    char curr[MAX_LEN];
    int ch, chpos, count, c;
    int correct = 0;
    char text[MAX_LEN];
    FILE *file;
    int word_count = 0;
    int level = 1;
    int lives;
    struct Result result;

    file = fopen(filename, "r");
    if (!file) {
        endwin();
        exit(0);
    }
    memset(text, 0, sizeof(text));
    lives = lives_counter(3, 0, cols);

    while ((c = getc(file)) != EOF) {
        if (isspace(c)) {
            if (strcmp(text, "") == 0) {
                continue;
            }
            ++word_count;
            if (word_count < level) {
                if (text[strlen(text)] != ' ') {
                    append(text, ' ');
                }
                continue;
            }
        } else {
            append(text, c);
            continue;
        }

        clear();
        refresh();

        chpos = 0;
        memset(curr, 0, sizeof(curr)); /* Reset current attemp */

        mvprintw(lines / 2, cols / 2, "%s", text);
        refresh();

        while (lives > 0) {
            ch = getch();

            if (ch == ERR || ch == KEY_EXIT || ch == 27) {
                /* For now, we end everything */
                endwin();
                exit(0);
            } else if (char_in_pos(text, ch, chpos) == 0) {
                /* Correct char */
                ++chpos;
                append(curr, ch);
                mvprintw(lines, cols / 2, curr);

                if (strcmp(curr, text) == 0) {
                    break;
                }

            } else {
                /* Incorrect char */
                lives = lives_counter(lives, -1, cols);
                if (lives == 0) {
                    break;
                }
                mvprintw(lines, cols / 2, curr);
            }
            refresh();
        }

        if (lives != 0) {
            ++correct;
        } else {
            break;
        }

        memset(text, 0, sizeof(text));
        word_count = 0;
    }

    result.lives = lives;
    result.correct = correct;

    return result;
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

int lives_counter(int lives, int amount, int maxcols){
    int new_lives;
    static WINDOW *lives_win;

    lives_win = newwin(1, 10, 0, maxcols - 10);
    new_lives = lives + amount;

    mvwprintw(lives_win, 0, 0, "Lives: %d", new_lives);
    wrefresh(lives_win);

    return new_lives;
}
