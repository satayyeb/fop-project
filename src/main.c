#include <SDL.h>
#include <SDL2_gfxPrimitives.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>


#ifdef main
#undef main
#endif

#define max_land_size  80
#define min_land_size  50


const int FPS = 60;
const int section = 50;
const int SCREEN_WIDTH = 20 * section;
const int SCREEN_HEIGHT = 13 * section;

typedef struct APP {
    SDL_Window *window;
    SDL_Surface *surface;
    SDL_Renderer *renderer;
    SDL_Texture *texture;
} APP;

typedef struct POINT {
    int x;
    int y;
    int r;
    int ownership;
} POINT;

int bad_distance(POINT first, POINT second) {
    if (((int) (sqrt(pow(first.x - second.x, 2) + pow(first.y - second.y, 2)))) < (first.r + second.r))
        return 1;
    return 0;
}

int random_between(int start, int end) {
    return start + rand() % (end - start + 1);
}

void draw_the_map(APP *app, POINT *array, int number_of_points) {
    for (int i = 0; i < number_of_points; i++) {
        if (array[i].ownership == 1) {
            filledCircleColor(app->renderer, array[i].x, array[i].y, array[i].r, 0xffde3ec9);
            filledCircleColor(app->renderer, array[i].x, array[i].y, 20, 0xaa702666);
        } else if (array[i].ownership == 2) {
            filledCircleColor(app->renderer, array[i].x, array[i].y, array[i].r, 0xff3535e8);
            filledCircleColor(app->renderer, array[i].x, array[i].y, 20, 0xaa09099e);
        } else {
            filledCircleColor(app->renderer, array[i].x, array[i].y, array[i].r, 0xffb7edba);
            filledCircleColor(app->renderer, array[i].x, array[i].y, 20, 0xaa2e8233);
        }
    }
}

int generate_random_map(POINT array[15], APP *app) {
    int number_of_points = random_between(10, 15);
    for (int i = 0; i < number_of_points; i++) {
        array[i].x = random_between(max_land_size, SCREEN_WIDTH - max_land_size);
        array[i].y = random_between(max_land_size, SCREEN_HEIGHT - max_land_size);
        array[i].r = min_land_size + rand() % (max_land_size - min_land_size);
        int j = 0;
        int number_of_fails_to_generate = 0;
        while (j < i) {
            if (bad_distance(array[i], array[j])) {
                array[i].x = random_between(max_land_size, SCREEN_WIDTH - max_land_size);
                array[i].y = random_between(max_land_size, SCREEN_HEIGHT - max_land_size);
                array[i].r = min_land_size + rand() % (max_land_size - min_land_size);
                number_of_fails_to_generate++;
                j = 0;
                if (number_of_fails_to_generate > 100) {
                    number_of_points = i + 1;
                    break;
                }
                continue;
            }
            number_of_fails_to_generate = 0;
            j++;
        }
    }
    array[0].ownership = 1;
    array[5].ownership = 2;
    draw_the_map(app, array, number_of_points);
    return number_of_points;
}

bool init(APP *app) {
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) < 0) {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return false;
    }
    app->window = SDL_CreateWindow("state.at", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH,
                                   SCREEN_HEIGHT,
                                   SDL_WINDOW_OPENGL);
    if (app->window == NULL) {
        printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
        return false;
    }
    app->renderer = SDL_CreateRenderer(app->window, -1, SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED);
    if (app->renderer == NULL) {
        printf("renderer could not be created! SDL_Error: %s\n", SDL_GetError());
        return false;
    }
    app->texture = SDL_CreateTexture(app->renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING,
                                     SCREEN_WIDTH, SCREEN_HEIGHT);
    app->surface = SDL_GetWindowSurface(app->window);
    return true;
}

void die(APP *app) {
    SDL_FreeSurface(app->surface);
    SDL_DestroyWindow(app->window);
    SDL_DestroyRenderer(app->renderer);
    SDL_DestroyTexture(app->texture);
    SDL_Quit();
    exit(-1);
}

int witch_point(int x, int y, POINT *array, int number_of_points) {
    POINT mouse = {x, y, 0};
    for (int i = 0; i < number_of_points; i++)
        if (bad_distance(array[i], mouse))
            return i;
    return -1;
}

int main() {
    APP *app = calloc(1, sizeof(APP));
    SDL_Event event;
    bool quit = false;
    srand(time(0));
    POINT array[15];

    if (!init(app)) {
        die(app);
    }


    SDL_SetRenderDrawColor(app->renderer, 252, 255, 217, 255);
    SDL_RenderClear(app->renderer);
    int number_of_points = generate_random_map(array, app);
    int starting_point = -1;


    while (!quit) {
//        SDL_Delay(10);
        SDL_PollEvent(&event);

        switch (event.type) {
            case SDL_QUIT:
                quit = true;
                break;

            case SDL_MOUSEBUTTONDOWN:
                if (event.button.button == SDL_BUTTON_LEFT) {
                    int point_number = witch_point(event.button.x, event.button.y, array, number_of_points);
                    if (array[point_number].ownership == 1)
                        starting_point = point_number;
                    else
                        starting_point = -1;
                }
                break;

            case SDL_MOUSEBUTTONUP:
                if (event.button.button == SDL_BUTTON_LEFT) {
                    int ending_point = witch_point(event.button.x, event.button.y, array, number_of_points);
                    if (ending_point != -1 && starting_point != -1) {
                        array[ending_point].ownership = 1;
                    }
                }
                starting_point = -1;
                break;
        }



        //TODO rendering code goes here

        // render window
//        SDL_RenderClear(app->renderer);
//        SDL_SetRenderDrawColor(app->renderer, 252, 255, 217, 255);

        draw_the_map(app, array, number_of_points);
        SDL_RenderPresent(app->renderer);
        SDL_Delay(10);
    }
    return 0;
}