#include <stdint.h>

#define BOARD_ROWS 6
#define BOARD_COLS 7

typedef struct board{
    int matrix[BOARD_ROWS][BOARD_COLS];
} board_t;

board_t *board_init(size_t item_size);

int get_disc(board_t board, int y, int x);

int place_disc(board_t *board, int x, int value);
int get_elements_in_column(board_t *board, int x);

int check_four(board_t *board, int start_x, int start_y);