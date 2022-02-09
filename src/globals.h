#ifndef ISLAND_SOLDIER_GLOBALS_H
#define ISLAND_SOLDIER_GLOBALS_H

#include <SDL.h>
#include <SDL2_gfxPrimitives.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <math.h>
#include <string.h>




#define DARK_GREEN      0xaa18851a
#define LIGHT_GREEN     0xff5fff5c
#define DARK_RED        0xaa09099e
#define LIGHT_RED       0xff3536e8
#define DARK_BLUE       0xaab83d14
#define LIGHT_BLUE      0xffED8E6D
#define max_land_size   80
#define min_land_size   50
#define ATTACK_LIMIT    15
#define section         50
#define SCREEN_WIDTH    20 * section
#define SCREEN_HEIGHT   13 * section



typedef struct APP {
    SDL_Window *window;
    SDL_Renderer *renderer;
    TTF_Font *font;
    char user[100];
    int coin;
    SDL_Texture *texture[17];
    SDL_Surface *surface[17];
} APP;

typedef struct COORDINATE {
    int x;
    int y;
} COORDINATE;

typedef struct POINT {
    int x;
    int y;
    int r;
    int value;
    int ownership;
} POINT;

typedef struct POT {
    int player1_pot_number;
    int player1_counter;
    int player2_pot_number;
    int player2_counter;
} POT;

typedef struct SOLDIER {
    int start_point;
    int end_point;
    int x;
    int y;
    int ownership;
    int launch_counter;
    int killed;
    int number_of_companions;
} SOLDIER;


#endif //ISLAND_SOLDIER_GLOBALS_H