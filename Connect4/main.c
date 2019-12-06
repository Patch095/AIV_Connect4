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

int error(const char *message){
    SDL_Log("%s: %s", message, SDL_GetError());
    SDL_Quit();
    return 1;
}

char* get_image(int value){
    if(value == 0){
        return EMPTY;
    }else if(value == 1){
        return RED;
    }else if(value == -1){
        return YELLOW;
    }
    return EMPTY;
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
    //set game loop
    uint8_t turn_played = 1;
    int game_loop = 0;
    //game loop
    while(game_loop == 0){
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
        SDL_RenderClear(renderer);



        //draw game board
        for(int y = 0; y < BOARD_ROWS; y++){
            for(int x = 0; x < BOARD_COLS; x++){
                int value = get_disc(game_board, y, x);
                char *file_name = get_image(value);
                //if(file_name != "error"){
                    int width, height, comp;
                    unsigned char *image = stbi_load(file_name, &width, &height, &comp, 4);               
                    if(!image){
                        return error("unable to load image");
                    }
                    SDL_Texture* texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_STREAMING, width, height);
                    if(!texture){
                        return error("unable to create");
                    }

                    Uint8 *pixels = NULL;
                    int pitch = 0;
                    if(SDL_LockTexture(texture, NULL, (void **)&pixels, &pitch)){
                        return error("unable to map texture into address space");
                    }
                    memcpy(pixels, image, pitch * height);
                    free(image);
                    SDL_UnlockTexture(texture);

                    SDL_Rect rect;
                    rect.x = x * width;
                    rect.y = y * height;
                    rect.w = width;
                    rect.h = height;
                    SDL_RenderCopy(renderer, texture, NULL, &rect);
                //}
            }
        }

        //get input

        //update board

        //check four

        //if win(break)
        //else {turn_played ++
        //disc_color == turn_played % 2
        //1 = 1, 0 = -1     5 == error }
    }

    if(game_loop == 1){
        SDL_Log("RED PLAYER WIN!");
    }else if(game_loop == -1){
        SDL_Log("YELLOW PLAYER WIN!");
    }

    //init SDL

 //loop
    //draw board with texture
    //for(y, x)
    //0 == draw empty, 1== draw red, -1 == draw yellow
    
    //get input 

    //check four

    //win || change player

    //turn_played ++
    //disc_color == turn_played % 2
    //1 = 1, 0 = -1     5 == error

    SDL_Quit();
    return 0;
}

//clang -c -o board.o .\board.c
//clang -I .\SDL2-2.0.10\include\ -o main.exe main.c -L .\SDL2-2.0.10\lib\x64\ -l SDL2 .\cpu/board.c