#include "board.h"
#include <string.h>
#include <stdlib.h>

#include <stdio.h>

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
                    //printf("value %d, x : %d, y : %d\n", value, x, i);
                    return i;
                }
            }
            return -1;  //column full
        }      
    }
    return -1;  //other error
}

int place_disc(board_t *board, int y, int x, int value){
    //check errors
    if(board){
        int max_x = BOARD_COLS;
        int max_y = BOARD_ROWS;
        if(0 <= x < max_x && y < max_y){
            //check if x is a valid input
            if(y == -1){
                return 0;   //column full
            }
            board->matrix[y][x] = value;
            return 1;
        }        
    }
    //if matrix[y][x] == 0 -> empty
    //if matrix[y][x] == 1 -> red
    //if matrix[y][x] == -1 -> yellow

    return 0;
}

int check_horizontal(board_t *board, int start_y, int start_x){
    //set counter 
    int counter = 1;

    int offset_left = 1;
    int offset_right = 1;

    int stop_left = 0;
    int stop_right = 0;
    //get your color
    int value = get_disc(board, start_y, start_x);
    int max_x = BOARD_COLS;
    printf("my colour %d, x %d - y %d\n", value, start_x, start_y);
    do{
        //unable to go left and right
        if(stop_left == 1 && stop_right == 1){
            break;
        }
        //check left
        int left_neighbour_index = start_x - offset_left;
        printf("next left: %d\n", left_neighbour_index);
        if(left_neighbour_index >= 0){  //neibour isn't a wall
            int left_neighbour = get_disc(board, start_y, left_neighbour_index);
            //if left_neighbour colour == your colour continue search
            printf("left colour %d\n", left_neighbour);

            if (value == left_neighbour){
                counter ++;
                printf("counter %d\n", counter);
                if(counter >= 4){
                    return value;   // win condition
                }
                offset_left++;
            }else{  //stop search left
                stop_left = 1;
            }            
        }else{  //neighbour is a wall
            stop_left = 1;
        }
        
        //check right
        int right_neighbour_index = start_x + offset_right;
        printf("next right: %d\n", right_neighbour_index);

        if(right_neighbour_index < max_x){
            int right_neighbour = get_disc(board, start_y, right_neighbour_index);
            printf("right colour %d\n", right_neighbour);
    
            if (value == right_neighbour){
                counter ++;
                printf("counter %d\n", counter);
                if(counter >= 4){
                    return value;   // win condition,
                }
                offset_right++;
                }else{  //stop search right
                    stop_right = 1;
                }            
        }else{
            stop_right = 1;
        }
        
    }while(1 > 0);
    return 0;   //nobody win and the game continues
}

int check_four(board_t *board, int start_y, int start_x){
    //check errors
    int max_x = BOARD_COLS;
    int max_y = BOARD_ROWS;
    if(board){
        if(0 <= start_x < max_x && 0 <= start_y < max_y){
            //check if x and y are a valid inputs
            int h_counter = check_horizontal(board, start_y, start_x);
            printf("horizontal = %d\n", h_counter);
            if(h_counter != 0){
                return h_counter;
            }
        }        
    }
    return 0;
}