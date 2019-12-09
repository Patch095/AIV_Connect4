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
    for(int i = 0; i < 4; i++){
        new_board->found_four_x[i] = -1;
        new_board->found_four_y[i] = -1;
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
    board->found_four_x[0] = start_x;
    board->found_four_y[0] = start_y;
    int counter = 1;

    int offset_left = 1;
    int offset_right = 1;

    int stop_left = 0;
    int stop_right = 0;
    //get your color
    int value = get_disc(board, start_y, start_x);
    int max_x = BOARD_COLS;
    do{
        //unable to go left and right
        if(stop_left == 1 && stop_right == 1){
            break;
        }
        //check left
        int left_neighbour_index = start_x - offset_left;
        if(left_neighbour_index >= 0){  //neibour isn't a wall
            int left_neighbour = get_disc(board, start_y, left_neighbour_index);
            //if left_neighbour colour == your colour continue search
            if (value == left_neighbour){
                board->found_four_y[counter] = start_y;
                board->found_four_x[counter] = left_neighbour_index;
                
                counter ++;
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
        if(right_neighbour_index < max_x){
            int right_neighbour = get_disc(board, start_y, right_neighbour_index);   
            if (value == right_neighbour){
                board->found_four_y[counter] = start_y;
                board->found_four_x[counter] = right_neighbour_index;

                counter ++;
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

int check_vertical(board_t *board, int start_y, int start_x){
    board->found_four_x[0] = start_x;
    board->found_four_y[0] = start_y;
    int counter = 1;

    int offset_bot = 1;

    int stop_bot = 0;

    int value = get_disc(board, start_y, start_x);
    do{
        if(stop_bot == 1){
            break;
        }
        //check bot
        int bot_neighbour_index = start_y - offset_bot;
        if(bot_neighbour_index >= 0){
            int bot_neighbour = get_disc(board, bot_neighbour_index, start_x);
            if (value == bot_neighbour){
                board->found_four_y[counter] = bot_neighbour_index;
                board->found_four_x[counter] = start_x;
                
                counter ++;
                if(counter >= 4){
                    return value;
                }
                offset_bot++;
            }else{
                stop_bot = 1;
            }            
        }else{
            stop_bot = 1;
        }           
    }while(1 > 0);
    return 0;
}

int check_diagonal_topleft_botright(board_t *board, int start_y, int start_x){
    board->found_four_x[0] = start_x;
    board->found_four_y[0] = start_y;
    int counter = 1;

    int offset_topleft = 1;
    int offset_botright = 1;

    int stop_topleft = 0;
    int stop_botright = 0;

    int value = get_disc(board, start_y, start_x);
    int max_x = BOARD_COLS;
    int max_y = BOARD_ROWS;
    do{
        //unable to go left and right
        if(stop_topleft == 1 && stop_botright == 1){
            break;
        }
        //check top left
        int left_neighbour_index = start_x - offset_topleft;
        int top_neighbour_index = start_y + offset_topleft;
        if(left_neighbour_index >= 0 && top_neighbour_index < max_y){
            int topleft_neighbour = get_disc(board, top_neighbour_index, left_neighbour_index);
            if (value == topleft_neighbour){
                board->found_four_y[counter] = top_neighbour_index;
                board->found_four_x[counter] = left_neighbour_index;
                
                counter ++;
                if(counter >= 4){
                    return value;
                }
                offset_topleft++;
            }else{
                stop_topleft = 1;
            }            
        }else{
            stop_topleft = 1;
        }   
        //check bot right
        int right_neighbour_index = start_x + offset_botright;
        int bot_neighbour_index = start_y - offset_botright;
        if(right_neighbour_index < max_x && bot_neighbour_index >= 0){
            int botright_neighbour = get_disc(board, bot_neighbour_index, right_neighbour_index);   
            if (value == botright_neighbour){
                board->found_four_y[counter] = bot_neighbour_index;
                board->found_four_x[counter] = right_neighbour_index;

                counter ++;
                if(counter >= 4){
                    return value;
                }
                offset_botright++;
                }else{
                    stop_botright = 1;
                }            
        }else{
            stop_botright = 1;
        }
        
    }while(1 > 0);
    return 0;
}

int check_diagonal_botleft_topright(board_t *board, int start_y, int start_x){
    board->found_four_x[0] = start_x;
    board->found_four_y[0] = start_y;
    int counter = 1;

    int offset_botleft = 1;
    int offset_topright = 1;

    int stop_botleft = 0;
    int stop_topright = 0;

    int value = get_disc(board, start_y, start_x);
    int max_x = BOARD_COLS;
    int max_y = BOARD_ROWS;
    do{
        //unable to go left and right
        if(stop_botleft == 1 && stop_topright == 1){
            break;
        }
        //check bot left
        int left_neighbour_index = start_x - offset_botleft;
        int bot_neighbour_index = start_y - offset_botleft;

        if(left_neighbour_index >= 0 && bot_neighbour_index >= 0){
            int botleft_neighbour = get_disc(board, bot_neighbour_index, left_neighbour_index);
            if (value == botleft_neighbour){
                board->found_four_y[counter] = bot_neighbour_index;
                board->found_four_x[counter] = left_neighbour_index;
                
                counter ++;
                if(counter >= 4){
                    return value;
                }
                offset_botleft++;
            }else{
                stop_botleft = 1;
            }            
        }else{
            stop_botleft = 1;
        }   
        //check bot right
        int right_neighbour_index = start_x + offset_topright; 
        int top_neighbour_index = start_y + offset_topright;

        if(right_neighbour_index < max_x && top_neighbour_index < max_y){
            int topright_neighbour = get_disc(board, top_neighbour_index, right_neighbour_index);   
            if (value == topright_neighbour){
                board->found_four_y[counter] = top_neighbour_index;
                board->found_four_x[counter] = right_neighbour_index;

                counter ++;
                if(counter >= 4){
                    return value;
                }
                offset_topright++;
                }else{
                    stop_topright = 1;
                }            
        }else{
            stop_topright = 1;
        }
        
    }while(1 > 0);
    return 0;
}

int check_four(board_t *board, int start_y, int start_x){
    //check errors
    int max_x = BOARD_COLS;
    int max_y = BOARD_ROWS;
    if(board){  //check if x and y are a valid inputs
        if(0 <= start_x < max_x && 0 <= start_y < max_y){
            //check horizontal
            int horizontal_counter = check_horizontal(board, start_y, start_x);
            if(horizontal_counter != 0){
                return horizontal_counter;
            }
            //check vertical
            int vertical_counter = check_vertical(board, start_y, start_x);
            if(vertical_counter != 0){
                return vertical_counter;
            }
            //check diagonal top_left/bot_right
            int topleft_botright_counter = check_diagonal_topleft_botright(board, start_y, start_x);
            if(topleft_botright_counter != 0){
                return topleft_botright_counter;
            }
            //check diagonal bot_left/top_right
            int topright_botleft_counter = check_diagonal_botleft_topright(board, start_y, start_x);
            if(topright_botleft_counter != 0){
                return topright_botleft_counter;
            }
        }        
    }
    for(int i = 0; i < 4; i++){
        board->found_four_x[i] = -1;
        board->found_four_y[i] = -1;
    }    
    return 0;
}