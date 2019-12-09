#include <stdint.h>

#define BOARD_ROWS 6    //y
#define BOARD_COLS 7    //x

typedef struct board{
    int matrix[BOARD_ROWS][BOARD_COLS];
    int found_four_x[4];
    int found_four_y[4];
} board_t;

board_t *board_init();

int get_disc(board_t *board, int y, int x);

int place_disc(board_t *board, int y, int x, int value);
int get_elements_in_column(board_t *board, int x);

int check_four(board_t *board, int start_y, int start_x);