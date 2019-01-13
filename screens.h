struct Result {
    int correct;
    int lives;
};

int end_screen(int lines, int cols, struct Result result);
int title_screen(int lines, int cols);
struct Result game_screen(int lines, int cols, char *filename, int max_chars);
int lives_counter(int lives, int amount, int maxcols);
