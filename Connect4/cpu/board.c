#include "board.h"
#include <string.h>
#include <stdlib.h>

board_t *board_init(){
    board_t *new_board = malloc(sizeof(board_t));
    if(!new_board){
        return NULL;
    }
    int max_x = BOARD_COLS;
    int max_y = BOARD_ROWS;
    for(int y = 0; y < max_y; y++){
        for(int x = 0; x < max_x; x++){
            new_board->matrix[y][x] = 0;
        }
    }
    return new_board;
}

int get_disc(board_t *board, int y, int x){
    int value = 5;
    //check errors
    if(board){
        int max_x = BOARD_COLS;
        int max_y = BOARD_ROWS;
        if(0 <= y < max_y | 0 <= x < max_x){
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
    if(board){
        int max_x = BOARD_COLS;
        int max_y = BOARD_ROWS;

        if(0 <= x < max_x){
            for(int i = 0; i < max_y; i++){
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
    if(board){
        int max_x = BOARD_COLS;
        if(0 <= x < max_x){
            //check if x is a valid input
            int index = get_elements_in_column(board, x);
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
        int max_x = BOARD_COLS;
        if(right_neighbour_index < max_x){
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

int check_four(board_t *board, int start_x, int start_y){
    //check errors
    int max_x = BOARD_COLS;
    int max_y = BOARD_ROWS;
    if(board){
        if(0 <= start_x < max_x && 0 <= start_y < max_y){
            //check if x and y are a valid inputs
            int h_counter = check_horizontal(board, start_x, start_y);
            if(h_counter != 0){
                return h_counter;
            }
        }        
    }
    return 0;
}