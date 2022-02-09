#include "globals.h"
#include "init.h"

bool init(APP *app) {
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) < 0) {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return false;
    }
    app->window = SDL_CreateWindow("island soldiers", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH,
                                   SCREEN_HEIGHT,
                                   SDL_WINDOW_OPENGL);
    if (app->window == NULL) {
        printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
        return false;
    }
    app->renderer = SDL_CreateRenderer(app->window, -1, SDL_RENDERER_ACCELERATED);
    if (app->renderer == NULL) {
        printf("renderer could not be created! SDL_Error: %s\n", SDL_GetError());
        return false;
    }

    if (TTF_Init() != 0) {
        printf("can not init SDL_TTF :( SDL_Error: %s\n", SDL_GetError());
        return false;
    }

    int flags = IMG_INIT_JPG | IMG_INIT_PNG;
    int init = IMG_Init(flags);
    if (init & flags != flags) {
        printf("IMG_Init: Failed to init required jpg and png support!\n");
        printf("IMG_Init: %s\n", IMG_GetError());
        return false;
    }

    return true;
}

void die(APP *app) {
    //free memory and exit
    for (int i = 0; i < 17; i++) {
        if (app->texture[i] != NULL)
            SDL_DestroyTexture(app->texture[i]);
        if (app->surface[i] != NULL)
            SDL_FreeSurface(app->surface[i]);
    }
    SDL_DestroyRenderer(app->renderer);
    SDL_DestroyWindow(app->window);
    IMG_Quit();
    TTF_Quit();
    SDL_Quit();
    free(app);
    exit(0);
}

