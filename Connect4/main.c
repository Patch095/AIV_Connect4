#define SDL_MAIN_HANDLED

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <SDL.h>

#include "cpu/board.h"

int error(const char *message){
    SDL_Log("%s: %s", message, SDL_GetError());
    SDL_Quit();
    return 1;
}

int main(int argc, char **argv){
    uint8_t turn_played = 1;


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

//clang -I .\SDL2-2.0.10\include\ -o main.exe main.c -L .\SDL2-2.0.10\lib\x64\ -l SDL2