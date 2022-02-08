#include "globals.h"
#include "map.h"
#include "menu.h"


bool present_first_screen(APP *app, SDL_Texture *image_texture) {

    bool shift = false;
    bool lock = false;
    bool quit = false;
    char ch[3] = {'\0'};
    char text[100] = {'\0'};


    SDL_Color text_color = {0, 0, 0, 255};
    SDL_Color text_color_white = {255, 255, 255, 255};
    SDL_Color background_color = {10, 10, 10, 1};
    int text_width, text_height;

    app->font = TTF_OpenFont("../fonts/ariblk.ttf", 70);
    TTF_SizeText(app->font, "ISLAND SOLDIERS", &text_width, &text_height);
    SDL_Rect textbox_huge = {(SCREEN_WIDTH - text_width) / 2, 50, text_width, text_height};
    SDL_Surface *surface_huge = TTF_RenderText_Shaded(app->font, "ISLAND SOLDIERS", text_color, background_color);
    SDL_Texture *texture_huge = SDL_CreateTextureFromSurface(app->renderer, surface_huge);
    if (texture_huge == NULL) {
        printf("null huge :(");
        exit(-1);
    }


    app->font = TTF_OpenFont("../fonts/ariblk.ttf", 25);
    TTF_SizeText(app->font, "Enter your name:", &text_width, &text_height);
    SDL_Rect textbox_medium = {(SCREEN_WIDTH - text_width) / 2, 190, text_width, text_height};
    SDL_Surface *surface_medium = TTF_RenderText_Shaded(app->font, "Enter your name:", text_color, background_color);
    SDL_Texture *texture_medium = SDL_CreateTextureFromSurface(app->renderer, surface_medium);
    if (texture_medium == NULL) {
        printf("null medium :(");
        exit(-1);
    }


    //    app->font = TTF_OpenFont("../fonts/ariblk.ttf", 25);
    TTF_SizeText(app->font, "Let's go!", &text_width, &text_height);
    SDL_Rect textbox_white = {(SCREEN_WIDTH - text_width) / 2, 430, text_width, text_height};
    SDL_Surface *surface_white = TTF_RenderText_Shaded(app->font, "Let's go!", text_color_white, background_color);
    SDL_Texture *texture_white = SDL_CreateTextureFromSurface(app->renderer, surface_white);
    if (texture_white == NULL) {
        printf("null white :(");
        exit(-1);
    }


    //    app->font = TTF_OpenFont("../fonts/ariblk.ttf", 30);
    //TTF_SizeText(app->font, text, &text_width, &text_height);
    SDL_Rect name_rect;// = {(SCREEN_WIDTH - text_width) / 2, 250, text_width, text_height};
    SDL_Surface *name_surface;// = TTF_RenderText_Shaded(app->font, text, text_color, background_color);
    SDL_Texture *name_texture;// = SDL_CreateTextureFromSurface(app->renderer, name_surface);

    //    app->font = TTF_OpenFont("../fonts/ariblk.ttf", 30);
    //    SDL_Surface *surface_small = TTF_RenderText_Shaded(app->font, text, text_color, background_color);
    //    SDL_Texture *texture_small = SDL_CreateTextureFromSurface(app->renderer, surface_small);
    //    if (name_texture == NULL) {
    //        printf("null small 1 :(");
    //        exit(-1);
    //    }


    bool render = 1;
    SDL_Event event;
    while (!quit) {
        if (render) {
            SDL_RenderCopy(app->renderer, image_texture, NULL, NULL);

            roundedRectangleRGBA(app->renderer, 200, 250, 800, 300, 10, 0, 0, 0, 255);

            SDL_RenderCopy(app->renderer, texture_huge, NULL, &textbox_huge);
            SDL_RenderCopy(app->renderer, texture_medium, NULL, &textbox_medium);


            if (strlen(text) > 0) {
                TTF_SizeText(app->font, text, &text_width, &text_height);
                name_rect.x = (SCREEN_WIDTH - text_width) / 2;
                name_rect.y = 250;
                name_rect.w = text_width;
                name_rect.h = text_height;
                name_surface = TTF_RenderText_Shaded(app->font, text, text_color, background_color);
                name_texture = SDL_CreateTextureFromSurface(app->renderer, name_surface);
                if (name_texture == NULL) {
                    printf("null small 2 :(");
                    exit(-1);
                }
                SDL_RenderCopy(app->renderer, name_texture, NULL, &name_rect);
            }


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
                        render = 1;
                        if (event.key.keysym.sym == SDLK_RETURN && strlen(text) > 0) {
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
                                int x_mouse, y_mouse;
                                SDL_GetMouseState(&x_mouse, &y_mouse);

                                if ((strlen(text) > 0) && (400 < x_mouse) && (x_mouse < 600) && (420 < y_mouse) &&
                                (y_mouse < 480)) {
                                    quit = true;
                                }
                            }
                            break;
        }

    }


    //users files
    char str[100];
    int coin = 0;
    FILE *file = fopen("../data/users.dat", "ab+");
    for (int i = 0; i < 50; i++) {
        fread(&coin, sizeof(int),1,file);
        if (feof(file)) {
            strcpy(app->user,text);
            app->coin = 100;
            fwrite(&coin,sizeof(int),1,file);
            fwrite(text,1,100,file);
            break;
        }
        fread(str,1,100,file);
        if (strcmp(str, text) == 0) {
            strcpy(app->user,text);
            app->coin = coin;
            break;
        }
    }
    fclose(file);



    SDL_DestroyTexture(texture_huge);
    SDL_FreeSurface(surface_huge);
    printf("huge freed successfully\n");
    SDL_DestroyTexture(texture_medium);
    SDL_FreeSurface(surface_medium);
    printf("medium freed successfully\n");
    SDL_DestroyTexture(name_texture);
    SDL_FreeSurface(name_surface);
    printf("name freed successfully\n");
    SDL_DestroyTexture(texture_white);
    SDL_FreeSurface(surface_white);
    printf("white freed successfully\n");

    return 1;
}


bool select_map(APP *app, POINT *array, int *number_of_points) {

    SDL_Surface *image = IMG_Load("../media/sea3.jpg");
    SDL_Texture *image_texture = SDL_CreateTextureFromSurface(app->renderer, image);
    printf("\nloaded image -> ok\n");
    SDL_Color text_color_white = {255, 255, 255, 255};
    SDL_Color background_color = {0, 0, 0, 1};
    int text_width, text_height;
    //    app->font = TTF_OpenFont("../fonts/ariblk.ttf", 25);

    TTF_SizeText(app->font, "next map", &text_width, &text_height);
    SDL_Rect rect1 = {(SCREEN_WIDTH - text_width) / 2 + 300, SCREEN_HEIGHT - 100, text_width, text_height};
    SDL_Surface *surface1 = TTF_RenderText_Shaded(app->font, "next map", text_color_white, background_color);
    SDL_Texture *texture1 = SDL_CreateTextureFromSurface(app->renderer, surface1);
    printf("sec 1");
    TTF_SizeText(app->font, "previous map", &text_width, &text_height);
    SDL_Rect rect2 = {(SCREEN_WIDTH - text_width) / 2 - 320, SCREEN_HEIGHT - 100, text_width, text_height};
    SDL_Surface *surface2 = TTF_RenderText_Shaded(app->font, "previous map", text_color_white, background_color);
    SDL_Texture *texture2 = SDL_CreateTextureFromSurface(app->renderer, surface2);
    printf("sec 2");
    TTF_SizeText(app->font, "generate random map", &text_width, &text_height);
    SDL_Rect rect3 = {(SCREEN_WIDTH - text_width) / 2, SCREEN_HEIGHT - 100, text_width, text_height};
    SDL_Surface *surface3 = TTF_RenderText_Shaded(app->font, "generate random map", text_color_white, background_color);
    SDL_Texture *texture3 = SDL_CreateTextureFromSurface(app->renderer, surface3);
    printf("sec 3");
    TTF_SizeText(app->font, "START", &text_width, &text_height);
    SDL_Rect rect4 = {(SCREEN_WIDTH - text_width) / 2, SCREEN_HEIGHT - 180, text_width, text_height};
    SDL_Surface *surface4 = TTF_RenderText_Shaded(app->font, "START", text_color_white, background_color);
    SDL_Texture *texture4 = SDL_CreateTextureFromSurface(app->renderer, surface4);
    printf("sec 4");

    SDL_Event event;

    char first_path[] = "../data/pre-made-maps/";
    char str[20] = {'\0'};
    int map_number = 1;
    bool free_and_exit = false;
    bool render = true;
    bool random_generated = false;
    bool lock = false;
    bool quit = false;

    while (!quit) {
        SDL_Delay(10);

        if (render) {

            if (!random_generated) {
                FILE *file = fopen("../data/pre-made-maps/maps-list.txt", "r");
                if (file == NULL) {
                    render = false;
                    printf("maps-list.txt not found \n");
                    continue;
                }
                for (int i = 1; i <= map_number; i++) {
                    fgets(str, 19, file);
                    if (feof(file)) {
                        map_number = i - 1;
                        break;
                    }
                }
                fclose(file);
                char path[100] = {'\0'};
                strcat(path, first_path);
                strcat(path, str);
                path[strlen(path) - 1] = '\0';
                file = fopen(path, "rb");
                if (file == NULL) {
                    printf("\n%s not found \n", path);
                    render = false;
                    continue;
                }
                fread(number_of_points, sizeof(*number_of_points), 1, file);
                fread(array, sizeof(array[0]), *number_of_points, file);
                fclose(file);
            }

            SDL_RenderClear(app->renderer);
            SDL_RenderCopy(app->renderer, image_texture, NULL, NULL);

            draw_the_map(app, array, *number_of_points);

            roundedBoxColor(app->renderer, 70, SCREEN_HEIGHT - 110, 290, SCREEN_HEIGHT - 50, 5, 0xddb03c30);
            roundedBoxColor(app->renderer, 330, SCREEN_HEIGHT - 110, 670, SCREEN_HEIGHT - 50, 5, 0xddb03c30);
            roundedBoxColor(app->renderer, 720, SCREEN_HEIGHT - 110, 880, SCREEN_HEIGHT - 50, 5, 0xddb03c30);
            roundedBoxColor(app->renderer, (SCREEN_WIDTH - text_width) / 2 - 20, SCREEN_HEIGHT - 190,
                            (SCREEN_WIDTH + text_width) / 2 + 20, SCREEN_HEIGHT - 130, 5, 0xddb03c30);

            SDL_RenderCopy(app->renderer, texture1, NULL, &rect1);
            SDL_RenderCopy(app->renderer, texture2, NULL, &rect2);
            SDL_RenderCopy(app->renderer, texture3, NULL, &rect3);
            SDL_RenderCopy(app->renderer, texture4, NULL, &rect4);

            SDL_RenderPresent(app->renderer);

            random_generated = false;
            render = false;
        }

        SDL_PollEvent(&event);
        switch (event.type) {
            case SDL_QUIT:
                free_and_exit = true;
                break;

                case SDL_MOUSEBUTTONUP:
                    lock = false;
                    break;

                    case SDL_MOUSEBUTTONDOWN:
                        if (!lock && event.button.button == SDL_BUTTON_LEFT) {
                            lock = true;
                            int x_mouse, y_mouse;
                            SDL_GetMouseState(&x_mouse, &y_mouse);
                            if (SCREEN_HEIGHT - 110 < y_mouse && y_mouse < SCREEN_HEIGHT - 50) {
                                if (70 < x_mouse && x_mouse < 290 && map_number > 1) { //prev
                                    map_number--;
                                    render = 1;
                                } else if (330 < x_mouse && x_mouse < 670) { //generate
                                    *number_of_points = generate_random_map(array, app);
                                    random_generated = true;
                                    render = 1;
                                } else if (720 < x_mouse && x_mouse < 880) { //next
                                    map_number++;
                                    render = 1;
                                }

                            } else if (SCREEN_HEIGHT - 190 < y_mouse &&
                            y_mouse < SCREEN_HEIGHT - 130 &&
                            (SCREEN_WIDTH - text_width) / 2 - 20 < x_mouse &&
                            x_mouse < (SCREEN_WIDTH + text_width) / 2 - 20) {    //start the game
                                quit = true;
                                break;
                            }
                        }
                        break;
        }

        if (quit || free_and_exit) {
            SDL_FreeSurface(surface1);
            SDL_FreeSurface(surface2);
            SDL_FreeSurface(surface3);
            SDL_FreeSurface(surface4);
            SDL_DestroyTexture(texture1);
            SDL_DestroyTexture(texture2);
            SDL_DestroyTexture(texture3);
            SDL_DestroyTexture(texture4);
            printf("\nall is right\n");
            printf("again all is right\n");

            SDL_FreeSurface(image);
            SDL_DestroyTexture(image_texture);
            printf("tammat\n");

            quit = true;
        }
    }
    if (free_and_exit)
        return 0;
    else
        return 1;
}


bool present_second_screen(APP *app, SDL_Texture *image_texture, POINT *array, int *number_of_points, SOLDIER **soldiers) {

    SDL_RenderClear(app->renderer);
    printf("renderer cleared successfully\n");
    SDL_RenderCopy(app->renderer, image_texture, NULL, NULL);
    printf("image copied successfully\n");
    roundedBoxColor(app->renderer, 350, 180, SCREEN_WIDTH - 350, 270, 5, 0xddb03c30);
    roundedBoxColor(app->renderer, 350, 280, SCREEN_WIDTH - 350, 370, 5, 0xddb03c30);
    roundedBoxColor(app->renderer, 350, 380, SCREEN_WIDTH - 350, 470, 5, 0xddb03c30);
    printf("boxes rendered successfully\n");
    SDL_Color text_color_white = {255, 255, 255, 255};
    SDL_Color background_color = {0, 0, 0, 1};
    int text_width, text_height;

    //    app->font = TTF_OpenFont("../fonts/ariblk.ttf", 35);
    if (app->font == NULL) {
        printf("font can not open :(\n");
        exit(-1);
    }
    printf("df\n");
    TTF_SizeText(app->font, "NEW GAME", &text_width, &text_height);
    SDL_Rect rect1 = {(SCREEN_WIDTH - text_width) / 2, 205, text_width, text_height};
    SDL_Surface *surface1 = TTF_RenderText_Shaded(app->font, "NEW GAME", text_color_white, background_color);
    if (surface1 == NULL) {
        printf("surface 1 is null :(");
        exit(-1);
    }
    SDL_Texture *texture1 = SDL_CreateTextureFromSurface(app->renderer, surface1);
    if (texture1 == NULL) {
        printf("texture 1 is null :(");
        exit(-1);
    }


    printf("I still alive... 1");
    TTF_SizeText(app->font, "LOAD GAME", &text_width, &text_height);
    SDL_Rect rect2 = {(SCREEN_WIDTH - text_width) / 2, 305, text_width, text_height};
    SDL_Surface *surface2 = TTF_RenderText_Shaded(app->font, "LOAD GAME", text_color_white, background_color);
    SDL_Texture *texture2 = SDL_CreateTextureFromSurface(app->renderer, surface2);
    if (texture2 == NULL) {
        printf("texture 2 is null :(");
        exit(-1);
    }

    printf("I still alive... 2");
    TTF_SizeText(app->font, "SCORES", &text_width, &text_height);
    SDL_Rect rect3 = {(SCREEN_WIDTH - text_width) / 2, 405, text_width, text_height};
    SDL_Surface *surface3 = TTF_RenderText_Shaded(app->font, "SCORES", text_color_white, background_color);
    SDL_Texture *texture3 = SDL_CreateTextureFromSurface(app->renderer, surface3);
    if (texture3 == NULL) {
        printf("texture 3 is null :(");
        exit(-1);
    }


    printf("I still alive... 3");
    SDL_RenderCopy(app->renderer, texture1, NULL, &rect1);
    SDL_RenderCopy(app->renderer, texture2, NULL, &rect2);
    SDL_RenderCopy(app->renderer, texture3, NULL, &rect3);
    printf("I still alive... ghodrat ");
    SDL_RenderPresent(app->renderer);
    printf("rendered seccessfully");

    SDL_Event event;
    bool free_and_exit = false;
    bool quit = false;
    while (!quit) {
        SDL_Delay(10);
        SDL_PollEvent(&event);
        switch (event.type) {
            case SDL_QUIT:
                free_and_exit = true;
                break;

                case SDL_MOUSEBUTTONDOWN:
                    if (event.button.button == SDL_BUTTON_LEFT) {
                        int x_mouse, y_mouse;
                        SDL_GetMouseState(&x_mouse, &y_mouse);
                        if (350 < x_mouse && x_mouse < SCREEN_WIDTH - 350) {
                            if (180 < y_mouse && y_mouse < 270) { //new game
                                if (select_map(app, array, number_of_points)) {
                                    quit = true;
                                    break;
                                } else {
                                    free_and_exit = true;
                                    break;
                                }
                            } else if (280 < y_mouse && y_mouse < 370) { //load game
                                FILE *file = fopen("../data/data.dat", "rb");
                                if (file == NULL)
                                    break;
                                fread(number_of_points, sizeof(*number_of_points), 1, file);
                                fread(array, sizeof(array[0]), *number_of_points, file);
                                int count = 0;
                                for (int i = 0; i < ATTACK_LIMIT; i++) {
                                    fread(&count, sizeof(count), 1, file);
                                    if (feof(file)) {
                                        fclose(file);
                                        quit = true;
                                        break;
                                    }
                                    soldiers[i] = calloc(count, sizeof(SOLDIER));
                                    fread(soldiers[i], sizeof(SOLDIER), count, file);
                                }
                                fclose(file);
                                quit = true;
                            }

                        } else if (380 < y_mouse && y_mouse < 470) { //scores

                        }
                    }
                    break;
        }

        if (quit || free_and_exit) {
            SDL_FreeSurface(surface1);
            SDL_FreeSurface(surface2);
            SDL_FreeSurface(surface3);
            SDL_DestroyTexture(texture1);
            SDL_DestroyTexture(texture2);
            SDL_DestroyTexture(texture3);

            quit = true;
        }
    }
    if (free_and_exit)
        return 0;
    else
        return 1;
}

