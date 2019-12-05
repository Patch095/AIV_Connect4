#include "board.h"
#include <string.h>
#include <stdlib.h>

board_t *board_init(size_t item_size){
    board_t *new_board= malloc(sizeof(board_t));
    if(!new_board){
        return NULL;
    }
    for(int y = 0; y < BOARD_ROWS; y++){
        for(int x = 0; x < BOARD_COLS; x++){
            new_board->matrix[y][x] = 0;
        }
    }
    return new_board;
}

int get_disc(board_t *board, int y, int x){
    int value = 5;
    //check errors
    if(board && board->matrix){
        if(0 <= y < BOARD_ROWS | 0 <= x < BOARD_COLS){
            value = board->matrix[y][x];
        }        
    }
    //if matrix[y][x] == 0 -> empty
    //if matrix[y][x] == 1 -> red
    //if matrix[y][x] == -1 -> yellow

    return value;
}

int get_elements_in_column(board_t *board, int x){
    //check errors
    if(board && board->matrix){
        if(0 <= x < BOARD_COLS){
            for(int i = 0; i < BOARD_ROWS; i++){
                int value = board->matrix[i][x];
                if(value == 0){
                    return i;
                }
            }
            return -1;  //column full
        }      
    }
    return -1;  //other error
}

int place_disc(board_t *board, int x, int value){
    //check errors
    if(board && board->matrix){
        if(0 <= x < BOARD_COLS){
            //check if x is a valid input
            int index = get_elements_in_colum(board, x);
            if(index == -1){
                return 0;   //column full
            }
            board->matrix[index][x] = value;
            return 1;
        }        
    }
    //if matrix[y][x] == 0 -> empty
    //if matrix[y][x] == 1 -> red
    //if matrix[y][x] == -1 -> yellow

    return 0;
}

int check_four(board_t *board, int start_x, int start_y){
    //check errors
    if(board && board->matrix){
        if(0 <= start_x < BOARD_COLS && 0 <= start_y && BOARD_ROWS){
            //check if x and y are a valid inputs
            int h_counter = check_horizontal(board, start_x, start_y);
            if(h_counter != 0){
                return h_counter;
            }
        }        
    }

}

int check_horizontal(board_t *board, int start_x, int start_y){
    //set counter 
    int counter = 1;

    int offset_left, offset_right = 0;
    int stop_left, stop_right = 0;
    //get your color
    int value = get_disc(board, start_x, start_y);

    do{
        //unable to go left and right
        if(stop_left == 1 && stop_right == 1){
            break;
        }
        //check left
        //check if neighbour != wall
        int left_neighbour_index = start_x - offset_left;
        if(left_neighbour_index >= 0){
            int left_neighbour = get_disc(board, left_neighbour_index, start_y);
            //if left_neighbour colour == your colour continue search
            if (value == left_neighbour){
                counter ++;
                if(counter >= 4){
                    return value;   // win condition
                }
                offset_left++;
            }else{  //stop search left
                stop_left = 1;
            }            
        }
        stop_left = 1;

        //check right
        int right_neighbour_index = start_x + offset_right;
        if(right_neighbour_index < BOARD_COLS){
            int right_neighbour = get_disc(board, right_neighbour_index, start_y);
            if (value == right_neighbour){
                counter ++;
                if(counter >= 4){
                    return value;   // win condition,
                }
                offset_right++;
            }else{  //stop search left
                stop_right = 1;
            }            
        }
        stop_right = 1;
    }while(1 > 0);

    return 0;   //nobody win and the game continues
}