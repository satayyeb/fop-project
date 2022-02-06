#include <SDL.h>
#include <SDL2_gfxPrimitives.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <math.h>


#ifdef main
#undef main
#endif

#define max_land_size  80
#define min_land_size  50
#define ATTACK_LIMIT    15
#define DARK_GREEN 0xaa702666
#define LIGHT_GREEN 0xffde3ec9
#define DARK_RED 0xaa09099e
#define LIGHT_RED 0xff3536e8

const int FPS = 60;
const int section = 50;
const int SCREEN_WIDTH = 20 * section;
const int SCREEN_HEIGHT = 13 * section;

typedef struct APP {
    SDL_Window *window;
    SDL_Surface *surface;
    SDL_Renderer *renderer;
    SDL_Texture *texture;
    TTF_Font *font;
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

int bad_distance(POINT first, POINT second) {
    if (((int) (sqrt(pow(first.x - second.x, 2) + pow(first.y - second.y, 2)))) < (first.r + second.r))
        return 1;
    return 0;
}

int random_between(int start, int end) {
    return start + rand() % (end - start + 1);
}

void draw_the_map(APP *app, POINT *array, int number_of_points) {
    char str[6];
    for (int i = 0; i < number_of_points; i++) {
        if (array[i].ownership == 1) {
            filledCircleColor(app->renderer, array[i].x, array[i].y, array[i].r, LIGHT_GREEN);
            filledCircleColor(app->renderer, array[i].x, array[i].y, 20, DARK_GREEN);
        } else if (array[i].ownership == 2) {
            filledCircleColor(app->renderer, array[i].x, array[i].y, array[i].r, LIGHT_RED);
            filledCircleColor(app->renderer, array[i].x, array[i].y, 20, DARK_RED);
        } else {
            filledCircleColor(app->renderer, array[i].x, array[i].y, array[i].r, 0xffb7edba);
            filledCircleColor(app->renderer, array[i].x, array[i].y, 20, 0xaa2e8233);
        }
        itoa(array[i].value, str, 10);
        stringRGBA(app->renderer, array[i].x - 6, array[i].y - 3, str, 0, 0, 0, 255);
    }
}

int generate_random_map(POINT array[15], APP *app) {
    int number_of_points = random_between(10, 15);
    for (int i = 0; i < number_of_points; i++) {
        array[i].value = random_between(10, 10);
        array[i].x = random_between(max_land_size, SCREEN_WIDTH - max_land_size);
        array[i].y = random_between(max_land_size, SCREEN_HEIGHT - max_land_size);
        array[i].r = random_between(min_land_size, max_land_size);
        int j = 0;
        int number_of_fails_to_generate = 0;
        while (j < i) {
            if (bad_distance(array[i], array[j])) {
                number_of_fails_to_generate++;
                j = 0;
                //generate another one:
                array[i].x = random_between(max_land_size, SCREEN_WIDTH - max_land_size);
                array[i].y = random_between(max_land_size, SCREEN_HEIGHT - max_land_size);
                array[i].r = random_between(min_land_size, max_land_size);
                if (number_of_fails_to_generate > 100) {
                    //we can not generate more states :(
                    number_of_points = i + 1;
                    break;
                }
            } else {
                number_of_fails_to_generate = 0;
                j++;
            }
        }
    }
    array[0].ownership = 1;
    array[5].ownership = 2;
    array[5].value = 20;
    draw_the_map(app, array, number_of_points);
    return number_of_points;
}

bool init(APP *app) {
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) < 0) {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return false;
    }
    app->window = SDL_CreateWindow("state.SAT", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH,
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
    app->texture = SDL_CreateTexture(app->renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING,
                                     SCREEN_WIDTH, SCREEN_HEIGHT);
    app->surface = SDL_GetWindowSurface(app->window);

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
    SDL_FreeSurface(app->surface);
//    SDL_DestroyWindow(app->window);
    SDL_DestroyRenderer(app->renderer);
    SDL_DestroyTexture(app->texture);
    SDL_Quit();
    TTF_Quit();
    IMG_Quit();
    exit(-1);
}

int witch_point(int x, int y, POINT *array, int number_of_points) {
    POINT mouse = {x, y, 0};
    for (int i = 0; i < number_of_points; i++)
        if (bad_distance(array[i], mouse))
            return i;
    return -1;
}

int line_between_start_end(int x, float a, int xf, int yf) {
    //  yi - yf = a(xf - xi)
    //= yi = axf - axi + yf
    return (int) (a * (float) xf - a * (float) x + (float) yf);
}

void move_the_soldier(APP *app, POINT *array, SOLDIER **soldiers, int i, int j) {
    int start_x = array[soldiers[i][j].start_point].x;
    int start_y = array[soldiers[i][j].start_point].y;
    int dest_x = array[soldiers[i][j].end_point].x;
    int dest_y = array[soldiers[i][j].end_point].y;

    //calculate the line slope between start point and end point
    float a = (float) (start_y - dest_y) / (float) (dest_x - start_x);

    ///direction algorithm////
    if (start_y > dest_y) {
        if (soldiers[i][j].y > line_between_start_end(soldiers[i][j].x, a, dest_x, dest_y)) {
            soldiers[i][j].y -= 2;
        } else {
            soldiers[i][j].x += 2 * ((dest_x - start_x) / abs(dest_x - start_x));
        }
    } else {
        if (soldiers[i][j].y < line_between_start_end(soldiers[i][j].x, a, dest_x, dest_y)) {
            soldiers[i][j].y += 2;
        } else {
            soldiers[i][j].x += 2 * ((dest_x - start_x) / abs(dest_x - start_x));
        }
    }
    /////////////////////////
}


void move(APP *app, POINT *array, SOLDIER **soldiers) {
    //find a group of soldiers wants to move from a point to another point
    for (int i = 0; i < ATTACK_LIMIT; i++) {
        if (soldiers[i] == NULL)
            continue;

        //free memory if all soldiers killed or reached the destination
        for (int j = 0; j < soldiers[i][0].number_of_companions; j++) {
            if (soldiers[i][j].killed != 1)
                break;
            if (j == soldiers[i][0].number_of_companions - 1) {
                free(soldiers[i]);
                soldiers[i] = NULL;
                return;
            }
        }

        //loop till the end of soldiers in the sequence
        for (int j = 0; j < soldiers[i][0].number_of_companions; j++) {
            //ignore the soldier who killed before
            if (soldiers[i][j].killed == 1)
                continue;

            //the soldiers must start moving after each other not at the same time
            if (soldiers[i][j].launch_counter < 10) {
                soldiers[i][j].launch_counter += 1;
                break;
            } else if (soldiers[i][j].launch_counter == 10) {
                array[soldiers[i][j].start_point].value -= 1;
                soldiers[i][j].killed = 0;
                soldiers[i][j].launch_counter += 1;
            }

            //move the soldier and save its position
            move_the_soldier(app, array, soldiers, i, j);

            //the soldiers will be killed if they reached to each other
            for (int k = 0; k < ATTACK_LIMIT; k++) {
                if (soldiers[k] == NULL)
                    continue;
                for (int l = 0; l < soldiers[k][0].number_of_companions; l++) {
                    if (soldiers[k][l].ownership != soldiers[i][j].ownership &&
                        abs(soldiers[k][l].x - soldiers[i][j].x) < 10 &&
                        abs(soldiers[k][l].y - soldiers[i][j].y) < 10) {
                        soldiers[i][j].killed = 1;
                        soldiers[k][l].killed = 1;
                        soldiers[k][l].x = 0;
                        soldiers[i][j].x = 0;
                    }
                }
            }

            //if the soldier reached the destination
            int dest_x = array[soldiers[i][j].end_point].x;
            int dest_y = array[soldiers[i][j].end_point].y;
            if (abs(soldiers[i][j].x - dest_x) < 8 && abs(soldiers[i][j].y - dest_y) < 8) {
                soldiers[i][j].killed = true;
                if (array[soldiers[i][j].end_point].value == 0) {
                    array[soldiers[i][j].end_point].ownership = soldiers[i][j].ownership;
                    array[soldiers[i][j].end_point].value += 1;
                } else if (array[soldiers[i][j].end_point].ownership == soldiers[i][j].ownership) {
                    array[soldiers[i][j].end_point].value += 1;
                } else {
                    array[soldiers[i][j].end_point].value -= 1;
                }
            }
            //print the soldiers
            if (soldiers[i][j].killed != 1) {
                if (soldiers[i][j].ownership == 1)
                    filledCircleColor(app->renderer, soldiers[i][j].x, soldiers[i][j].y, 5, DARK_GREEN);
                else
                    filledCircleColor(app->renderer, soldiers[i][j].x, soldiers[i][j].y, 5, DARK_RED);
            }
        }
    }
}


void make_soldier(POINT *array, int number_of_points) {
    for (int i = 0; i < number_of_points; i++) {
        //        if (array[i].ownership == 0 && array[i].value < 10) {
        //            array[i].value += 1;
        //        }
        if (array[i].ownership == 1 && array[i].value < 50) {
            array[i].value += 1;
        }
        if (array[i].ownership == 2 && array[i].value < 80) {
            array[i].value += 1;
        }
    }
}

void arrow(APP *app, int x1, int y1, int x2, int y2, double width, Uint32 color) {
    //TODO correcting the arrow and mouse positions
    thickLineColor(app->renderer, x1, y1, x2, y2, width, color);
    double angle = atan((double) (y1 - y2) / (double) (x2 - x1));
    COORDINATE T[3]; //The triangle's sides
    T[0].x = x2 + 1.2 * sqrt(3) * width * cos(angle) * ((x2 - x1 == 0) ? 1 : ((x2 - x1) / abs(x2 - x1)));
    T[0].y = y2 - 1.2 * sqrt(3) * width * sin(angle) * ((x2 - x1 == 0) ? 1 : ((x2 - x1) / abs(x2 - x1)));
    T[1].x = x2 + 1.4 * width * sin(angle);
    T[1].y = y2 + 1.4 * width * cos(angle);
    T[2].x = x2 - 1.4 * width * sin(angle);
    T[2].y = y2 - 1.4 * width * cos(angle);
    filledTrigonColor(app->renderer, T[0].x, T[0].y, T[1].x, T[1].y, T[2].x, T[2].y, color);
}


void print_text(char *string, char *font_path, int size, SDL_Color color, SDL_Color background_color, int x, int y,
                APP *app) {

}

bool present_main_menu(APP *app) {

    char ch[2] = {'\0'};
    bool shift = false;
    bool lock = false;
    bool quit = false;
    char text[70] = {'\0'};


    SDL_Surface *image = IMG_Load("../media/island.jpg");
    SDL_Texture *texture = SDL_CreateTextureFromSurface(app->renderer, image);
//        SDL_Rect dstrect = { 200, 200, 50, 50 };
//        SDL_RenderCopy(renderer, texture, NULL, &dstrect);




    SDL_Color background_color = {10, 10, 10, 1};
    SDL_Color text_color = {0, 0, 0, 255};
    SDL_Color text_color_white = {255, 255, 255, 255};


    int text_width, text_height;
    TTF_Font *font_huge = TTF_OpenFont("../fonts/ariblk.ttf", 70);
    TTF_SizeText(font_huge, "ISLAND SOLDIERS", &text_width, &text_height);
    SDL_Rect textbox_huge = {(SCREEN_WIDTH - text_width) / 2, 50, text_width, text_height};
    SDL_Surface *surface_huge = TTF_RenderText_Shaded(font_huge, "ISLAND SOLDIERS", text_color, background_color);
    SDL_Texture *texture_huge = SDL_CreateTextureFromSurface(app->renderer, surface_huge);

    TTF_Font *font_medium = TTF_OpenFont("../fonts/ariblk.ttf", 35);
    TTF_SizeText(font_medium, "Enter your name:", &text_width, &text_height);
    SDL_Rect textbox_medium = {(SCREEN_WIDTH - text_width) / 2, 190, text_width, text_height};
    SDL_Surface *surface_medium = TTF_RenderText_Shaded(font_medium, "Enter your name:", text_color, background_color);
    SDL_Texture *texture_medium = SDL_CreateTextureFromSurface(app->renderer, surface_medium);


    TTF_Font *font_white = TTF_OpenFont("../fonts/ariblk.ttf", 25);
    TTF_SizeText(font_white, "Let's go!", &text_width, &text_height);
    SDL_Rect textbox_white = {(SCREEN_WIDTH - text_width) / 2, 430, text_width, text_height};
    SDL_Surface *surface_white = TTF_RenderText_Shaded(font_white, "Let's go!", text_color_white, background_color);
    SDL_Texture *texture_white = SDL_CreateTextureFromSurface(app->renderer, surface_white);


    TTF_Font *font_small = TTF_OpenFont("../fonts/ariblk.ttf", 30);
    SDL_Surface *surface_small = TTF_RenderText_Shaded(font_small, text, text_color, background_color);
    SDL_Texture *texture_small = SDL_CreateTextureFromSurface(app->renderer, surface_small);

    bool render = 1;
    SDL_Event event;
    while (!quit) {
        if(render){
            SDL_RenderCopy(app->renderer, texture, NULL, NULL);
            //        SDL_SetRenderDrawColor(app->renderer, 140, 55, 237, 255);
            //        SDL_RenderClear(app->renderer);

            stringRGBA(app->renderer, 200, 170, "Enter your name:", 0, 4, 4, 255);
            roundedRectangleRGBA(app->renderer, 200, 250, 800, 300, 10, 0, 0, 0, 255);


            SDL_RenderCopy(app->renderer, texture_huge, NULL, &textbox_huge);
            SDL_RenderCopy(app->renderer, texture_medium, NULL, &textbox_medium);


            TTF_SizeText(font_small, text, &text_width, &text_height);
            SDL_Rect textbox_small = {220, 250, text_width, text_height};
            surface_small = TTF_RenderText_Shaded(font_small, text, text_color, background_color);
            texture_small = SDL_CreateTextureFromSurface(app->renderer, surface_small);
            SDL_RenderCopy(app->renderer, texture_small, NULL, &textbox_small);


            roundedBoxColor(app->renderer, 400, 420, 600, 480, 20, 0xeeb03c30);
            SDL_RenderCopy(app->renderer, texture_white, NULL, &textbox_white);

            SDL_RenderPresent(app->renderer);

            render = 0;
        }
        SDL_Delay(10);
        SDL_PollEvent(&event);

        switch (event.type) {
            case SDL_QUIT:
                quit = true;
                return 0;
                break;

            case SDL_KEYUP:
                if (event.key.keysym.sym == SDLK_RSHIFT || event.key.keysym.sym == SDLK_LSHIFT) {
                    shift = false;
                }
                lock = false;
                break;

            case SDL_KEYDOWN:
                render =1;
                if (event.key.keysym.sym == SDLK_RETURN) {
                    quit = true;
                    break;
                } else if (event.key.keysym.sym == SDLK_RSHIFT || event.key.keysym.sym == SDLK_LSHIFT) {
                    shift = true;
                } else if (!lock && event.key.keysym.sym == SDLK_BACKSPACE && strlen(text) > 0) {
                    text[strlen(text) - 1] = '\0';
                    lock = true;
                } else if (event.key.repeat == 0 && (32 <= event.key.keysym.sym) && (event.key.keysym.sym <= 127) &&
                           text_width < 550) {
                    ch[0] = (char) event.key.keysym.sym;
                    if (shift && 'a' <= ch[0] && ch[0] <= 'z') {
                        ch[0] = (char) ((int) ch[0] - 32);
                    }
                    strcat(text, ch);
                    lock = true;
                }
                break;

            case SDL_MOUSEBUTTONDOWN:
                if (event.button.button == SDL_BUTTON_LEFT) {
//                    lock = 1;
                    int x_mouse, y_mouse;
                    SDL_GetMouseState(&x_mouse, &y_mouse);
                    if (400 < x_mouse && x_mouse < 600 && 420 < y_mouse && y_mouse < 480) {
                        quit = true;
                    }
                }
                break;
        }

    }
    SDL_FreeSurface(surface_huge);
    SDL_DestroyTexture(texture_huge);
    TTF_CloseFont(font_huge);

    SDL_FreeSurface(surface_medium);
    SDL_DestroyTexture(texture_medium);
    TTF_CloseFont(font_medium);

    SDL_FreeSurface(surface_small);
    SDL_DestroyTexture(texture_small);
    TTF_CloseFont(font_small);

    SDL_FreeSurface(surface_white);
    SDL_DestroyTexture(texture_white);
    TTF_CloseFont(font_white);

    SDL_FreeSurface(image);
    SDL_DestroyTexture(texture);

    return true;
}


int main(int argc, char *argv[]) {

    APP *app = calloc(1, sizeof(APP));
    SDL_Event event;
    bool quit = false;
    srand(time(0));
    POINT array[15] = {0};

    Uint32 color = 0xFFaaaaaa;


    if (!init(app)) {
        die(app);
    }

    if (!present_main_menu(app)) {
        return 0;
    }



    int number_of_points = generate_random_map(array, app);
    int starting_point = -1;
    SOLDIER *soldiers[ATTACK_LIMIT] = {NULL};
    int counter = 0;

    quit = false;
    while (!quit) {

        SDL_PollEvent(&event);


        SDL_SetRenderDrawColor(app->renderer, 252, 255, 217, 255);
        SDL_RenderClear(app->renderer);


        switch (event.type) {
            case SDL_QUIT:
                quit = true;
                break;

            case SDL_MOUSEBUTTONDOWN:
                if (event.button.button == SDL_BUTTON_LEFT && starting_point == -1) {
                    int point_number = witch_point(event.button.x, event.button.y, array, number_of_points);
                    if (array[point_number].ownership == 1 || array[point_number].ownership == 2) {
                        //                        printf("button down\n");
                        starting_point = point_number;
                    }
                }
                break;

            case SDL_MOUSEBUTTONUP:
                if (event.button.button == SDL_BUTTON_LEFT && starting_point != -1) {
                    int ending_point = witch_point(event.button.x, event.button.y, array, number_of_points);
                    if (ending_point != -1 && ending_point != starting_point) {
                        //                        printf("button up\n");


                        int already_reserved_soldiers = 0;
                        for (int i = 0; i < ATTACK_LIMIT; i++) {
                            if (soldiers[i] != NULL && soldiers[i][0].start_point == starting_point) {
                                for (int j = 0; j < soldiers[i][0].number_of_companions; ++j) {
                                    if (soldiers[i][j].killed == -1) {
                                        already_reserved_soldiers += 1;
                                    }
                                }
                            }
                        }
                        int number_of_soldiers = array[starting_point].value - already_reserved_soldiers;
                        int i = 0;
                        while (soldiers[i] != NULL && i < ATTACK_LIMIT)
                            i++;
                        soldiers[i] = calloc(number_of_soldiers + 1, sizeof(SOLDIER));
                        for (int j = 0; j < number_of_soldiers; j++) {
                            soldiers[i][j].ownership = array[starting_point].ownership;
                            soldiers[i][j].start_point = starting_point;
                            soldiers[i][j].end_point = ending_point;
                            soldiers[i][j].x = array[starting_point].x;
                            soldiers[i][j].y = array[starting_point].y;
                            soldiers[i][j].killed = -1;
                            soldiers[i][j].number_of_companions = number_of_soldiers;
                        }
                    }
                }
                starting_point = -1;
                break;
        }
        //TO//DO rendering code goes here
        // render window

        counter++;
        if (counter > 100) {
            counter = 0;
            make_soldier(array, number_of_points);
        }

        draw_the_map(app, array, number_of_points);
        move(app, array, soldiers);
        //render arrows
        if (starting_point != -1) {
            int x_mouse, y_mouse;
            SDL_GetMouseState(&x_mouse, &y_mouse);
            arrow(app, array[starting_point].x, array[starting_point].y, x_mouse, y_mouse, 15, color);
        }


        SDL_RenderPresent(app->renderer);
        SDL_Delay(5);
    }
    return 0;
}