#define SDL_MAIN_HANDLED
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <SDL.h>

#include <stdio.h>

#include "cpu/board.h"
//init textures
#define EMPTY "textures/empty.png"
#define RED "textures/player1.png"
#define YELLOW "textures/player2.png"

#define GREEN "textures/win.png"
#define RED_WIN "textures/red_win.png"
#define YELLOW_WIN "textures/yellow_win.png"

int error(const char *message){
    SDL_Log("%s: %s", message, SDL_GetError());
    SDL_Quit();
    return 1;
}

int play_turn(board_t* game_board, int x, int turn_played){
    int y = get_elements_in_column(game_board, x);
    if(y == -1){
        return 0;
    }
    int player_colour = turn_played % 2;
    if(player_colour == 0){
        player_colour = -1;
    }
    place_disc(game_board, y, x, player_colour);
    int value = get_disc(game_board, y, x);
    return check_four(game_board, y, x);
}

int main(int argc, char **argv){
    //SDL INIT
    if(SDL_Init(SDL_INIT_VIDEO)){
        SDL_Log("error initializing SDL2: %s", SDL_GetError());
    }
    SDL_Window* window = SDL_CreateWindow("Connect Four", 100, 100, 1600, 900, SDL_WINDOW_SHOWN);
    if(!window){
        return error("error creating window: %s");
    }
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED);
    if(!renderer){
        return error("unable to create renderer");
    }
    //Init game board
    board_t* game_board = board_init();
    if(!game_board){
        SDL_Log("error create board");
        SDL_Quit();
        return 1;
    }

    //draw game board
    //image datas
    int width, height, comp;
    int win_width, win_height, win_comp;
    unsigned char *image_empty = stbi_load(EMPTY, &width, &height, &comp, 4);
    unsigned char *image_red = stbi_load(RED, &width, &height, &comp, 4);               
    unsigned char *image_yellow = stbi_load(YELLOW, &width, &height, &comp, 4);  
    
    unsigned char *image_green = stbi_load(GREEN, &width, &height, &comp, 4);  
    unsigned char *image_red_win = stbi_load(RED_WIN, &win_width, &win_height, &win_comp, 4);  
    unsigned char *image_yellow_win = stbi_load(YELLOW_WIN, &win_width, &win_height, &win_comp, 4);  

    if(!image_empty | !image_red |!image_yellow |!image_green |!image_red_win |!image_yellow_win){
        return error("unable to load image");
    }

    SDL_Texture* empty = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_STREAMING, width, height);
    SDL_Texture* red = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_STREAMING, width, height);
    SDL_Texture* yellow = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_STREAMING, width, height);
    
    SDL_Texture* green = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_STREAMING, width, height);
    SDL_Texture* red_win = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_STREAMING, win_width, win_height);
    SDL_Texture* yellow_win = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_STREAMING, win_width, win_height);

    if(!empty | !red | !yellow |!green |!red_win |!yellow_win){
        return error("unable to create");
    }

    //load game textures
    Uint8 *pixels = NULL;
    int pitch = 0;
    //empty texture
    if(SDL_LockTexture(empty, NULL, (void **)&pixels, &pitch)){
        return error("unable to map texture into address space");
    }
    memcpy(pixels, image_empty, pitch * height);
    free(image_empty);
    SDL_UnlockTexture(empty);
    //red texture
    if(SDL_LockTexture(red, NULL, (void **)&pixels, &pitch)){
        return error("unable to map texture into address space");
    }
    memcpy(pixels, image_red, pitch * height);
    free(image_red);
    SDL_UnlockTexture(red);
    //yellow textures
    if(SDL_LockTexture(yellow, NULL, (void **)&pixels, &pitch)){
        return error("unable to map texture into address space");
    }
    memcpy(pixels, image_yellow, pitch * height);
    free(image_yellow);
    SDL_UnlockTexture(yellow);
    //green textures
    if(SDL_LockTexture(green, NULL, (void **)&pixels, &pitch)){
        return error("unable to map texture into address space");
    }
    memcpy(pixels, image_green, pitch * height);
    free(image_green);
    SDL_UnlockTexture(green);
    //red win textures
    if(SDL_LockTexture(red_win, NULL, (void **)&pixels, &pitch)){
        return error("unable to map texture into address space");
    }
    memcpy(pixels, image_red_win, pitch * win_height);
    free(image_red_win);
    SDL_UnlockTexture(red_win);
    //yellow win textures
    if(SDL_LockTexture(yellow_win, NULL, (void **)&pixels, &pitch)){
        return error("unable to map texture into address space");
    }
    memcpy(pixels, image_yellow, pitch * win_height);
    free(image_yellow_win);
    SDL_UnlockTexture(yellow_win);

    //init rects
    SDL_Rect rects[BOARD_ROWS * BOARD_COLS];
    int max_y = BOARD_ROWS;
    int max_x = BOARD_COLS;
    int offset = 100;
    int i = 0;
    for(int y = 0; y < max_y; y++){
        for(int x = 0; x < max_x; x++){
            rects[i].x = (x * width) + offset;
            rects[i].y = (max_y * height) -  (y * height) - offset/2;
            rects[i].w = width;
            rects[i].h = height;
            i++;
        }
    }
    SDL_Rect winner_rect;
    winner_rect.x = 1360;
    winner_rect.y = 400;
    winner_rect.w = win_width;
    winner_rect.h = win_height;

    //set game loop
    uint8_t turn_played = 1;
    int player_colour = 0;
    int turn = 0;
    int player_win = 0;
   
    //game loop
    for(;;){
        //get input
        SDL_Event event;
        while(SDL_PollEvent(&event)){//escape
            if(event.type == SDL_QUIT){
                SDL_Quit();
                return 0;
            }//input
            else if(event.type == SDL_KEYUP){
                if(event.key.keysym.sym == SDLK_ESCAPE){
                    SDL_Quit();
                    return 0;                    
            }else if(event.type == SDL_KEYUP && player_win == 0){
                switch (event.key.keysym.sym){
                    case SDLK_1:
                        turn = play_turn(game_board, 0, turn_played);
                        if(turn == 0){
                            turn_played++;
                        }else{
                            for(int i = 0; i < 4; i++){
                                int x = game_board->found_four_x[i];
                                int y = game_board->found_four_y[i];
                                SDL_Log("x %d, y %d\n", x, y);
                                place_disc(game_board, y, x, 10);
                                SDL_Log("%d\n", get_disc(game_board, y, x));
                            }
                            player_win = turn;
                        }
                        break;
                    case SDLK_2:
                        turn = play_turn(game_board, 1, turn_played);
                        if(turn == 0){
                            turn_played++;
                        }else{
                            for(int i = 0; i < 4; i++){
                                int x = game_board->found_four_x[i];
                                int y = game_board->found_four_y[i];
                                SDL_Log("x %d, y %d\n", x, y);
                                place_disc(game_board, y, x, 10);
                                SDL_Log("%d\n", get_disc(game_board, y, x));
                            }
                            player_win = turn;
                        }
                        break;
                    case SDLK_3:
                        turn = play_turn(game_board, 2, turn_played);
                        if(turn == 0){
                            turn_played++;
                        }else{
                            for(int i = 0; i < 4; i++){
                                int x = game_board->found_four_x[i];
                                int y = game_board->found_four_y[i];
                                SDL_Log("x %d, y %d\n", x, y);
                                place_disc(game_board, y, x, 10);
                                SDL_Log("%d\n", get_disc(game_board, y, x));
                            }
                            player_win = turn;
                        }
                        break;
                    case SDLK_4:
                        turn = play_turn(game_board, 3, turn_played);
                        if(turn == 0){
                            turn_played++;
                        }else{
                            for(int i = 0; i < 4; i++){
                                int x = game_board->found_four_x[i];
                                int y = game_board->found_four_y[i];
                                SDL_Log("x %d, y %d\n", x, y);
                                place_disc(game_board, y, x, 10);
                                SDL_Log("%d\n", get_disc(game_board, y, x));
                            }
                            player_win = turn;
                        }
                        break;
                    case SDLK_5:
                        turn = play_turn(game_board, 4, turn_played);
                        if(turn == 0){
                            turn_played++;
                        }else{
                            for(int i = 0; i < 4; i++){
                                int x = game_board->found_four_x[i];
                                int y = game_board->found_four_y[i];
                                SDL_Log("x %d, y %d\n", x, y);
                                place_disc(game_board, y, x, 10);
                                SDL_Log("%d\n", get_disc(game_board, y, x));
                            }
                            player_win = turn;
                        }
                        break; 
                    case SDLK_6:
                        turn = play_turn(game_board, 5, turn_played);
                        if(turn == 0){
                            turn_played++;
                        }else{
                            for(int i = 0; i < 4; i++){
                                int x = game_board->found_four_x[i];
                                int y = game_board->found_four_y[i];
                                SDL_Log("x %d, y %d\n", x, y);
                                place_disc(game_board, y, x, 10);
                                SDL_Log("%d\n", get_disc(game_board, y, x));
                            }
                            player_win = turn;
                        }
                        break; 
                    case SDLK_7:
                        turn = play_turn(game_board, 6, turn_played);
                        if(turn == 0){
                            turn_played++;
                        }else{
                            for(int i = 0; i < 4; i++){
                                int x = game_board->found_four_x[i];
                                int y = game_board->found_four_y[i];
                                SDL_Log("x %d, y %d\n", x, y);
                                place_disc(game_board, y, x, 10);
                                SDL_Log("%d\n", get_disc(game_board, y, x));
                            }
                            player_win = turn;
                        }
                        break;
                    default:
                        break;
                    }
                }
            }
        }
        //draw game board
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
        SDL_RenderClear(renderer);

        int max_y = BOARD_ROWS;
        int max_x = BOARD_COLS;
        int i = 0;
        for(int y = 0; y < max_y; y++){
            for(int x = 0; x < max_x; x++){
                int value = get_disc(game_board, y, x);
                if(value == 10){
                    SDL_RenderCopy(renderer, green, NULL, &rects[i]);
                }else if(value == 1){
                    SDL_RenderCopy(renderer, red, NULL, &rects[i]);
                }else if(value == -1){
                    SDL_RenderCopy(renderer, yellow, NULL, &rects[i]);
                }else{
                    SDL_RenderCopy(renderer, empty, NULL, &rects[i]);
                }
                i++;
            }
        }
        //game end
        if(player_win == 1){
            SDL_RenderCopy(renderer, red_win, NULL, &winner_rect);
            //SDL_Log("RED PLAYER WIN!");
        }else if(player_win == -1){
            SDL_RenderCopy(renderer, yellow_win, NULL, &winner_rect);
            //SDL_Log("YELLOW PLAYER WIN!");
        }
        //screen update
        SDL_RenderPresent(renderer);
    }

    SDL_Quit();
    return 0;
}

//clang -c -o board.o .\board.c
//clang -I .\SDL2-2.0.10\include\ -o main.exe main.c -L .\SDL2-2.0.10\lib\x64\ -l SDL2 .\cpu/board.c