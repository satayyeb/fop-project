#include "globals.h"
#include "map.h"
#include "menu.h"


bool show_scores(APP *app, SDL_Texture *image_texture) {

    SDL_RenderClear(app->renderer);

    SDL_RenderCopy(app->renderer, image_texture, NULL, NULL);
    roundedBoxColor(app->renderer, 350, 200, SCREEN_WIDTH - 350, 500, 5, 0xddb03c30);
    stringRGBA(app->renderer, 400, 480, "press any key to return...", 255, 255, 255, 255);

    char users[17][100];
    int coin[17];
    int number_of_users = 0;
    FILE *file = fopen("../data/users.dat", "rb");
    if (file == NULL) {
        printf("can not open users file in scores");
        return 0;
    }
    fread(&number_of_users, sizeof(number_of_users), 1, file);
    if (number_of_users > 17)
        number_of_users = 17;
    for (int i = 0; i < number_of_users; i++) {
        fread(users[i], 1, 100, file);
        fread(&coin[i], sizeof(int), 1, file);
    }
    fclose(file);

    //sort the table
    int sort[17] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16};
    if (number_of_users > 1) {
        for (int i = 0; i < number_of_users - 1; i++) {
            for (int j = i; j < number_of_users; j++) {
                if (coin[sort[i]] < coin[sort[j]]) {
                    int buff = sort[i];
                    sort[i] = sort[j];
                    sort[j] = buff;
                }
            }
        }
    }

    //print the table
    char str[5] = {'\0'};
    for (int i = 0; i < number_of_users; i++) {
        stringRGBA(app->renderer, 400, 220 + i * 15, users[sort[i]], 255, 255, 255, 255);
        sprintf(str, "%d", coin[sort[i]]);
        stringRGBA(app->renderer, 580, 220 + i * 15, str, 255, 255, 255, 255);
    }

    SDL_RenderPresent(app->renderer);

    SDL_Event event;
    while (true) {

        SDL_Delay(30);
        SDL_PollEvent(&event);

        switch (event.type) {
            case SDL_QUIT:
                return 0;

            case SDL_KEYDOWN:
                return 1;
        }
    }
}


bool present_first_screen(APP *app, SDL_Texture *image_texture) {

    SDL_Color text_color = {0, 0, 0, 255};
    SDL_Color text_color_white = {255, 255, 255, 255};
    SDL_Color background_color = {10, 10, 10, 1};
    int text_width, text_height;

    app->font = TTF_OpenFont("../fonts/ariblk.ttf", 70);
    TTF_SizeText(app->font, "ISLAND SOLDIERS", &text_width, &text_height);
    SDL_Rect textbox_huge = {(SCREEN_WIDTH - text_width) / 2, 50, text_width, text_height};
    app->surface[0] = TTF_RenderText_Shaded(app->font, "ISLAND SOLDIERS", text_color, background_color);
    app->texture[0] = SDL_CreateTextureFromSurface(app->renderer, app->surface[0]);
    if (app->texture[0] == NULL) {
        printf("null huge :(");
        exit(-1);
    }


    app->font = TTF_OpenFont("../fonts/ariblk.ttf", 25);
    TTF_SizeText(app->font, "Enter your name:", &text_width, &text_height);
    SDL_Rect textbox_medium = {(SCREEN_WIDTH - text_width) / 2, 190, text_width, text_height};
    app->surface[1] = TTF_RenderText_Shaded(app->font, "Enter your name:", text_color, background_color);
    app->texture[1] = SDL_CreateTextureFromSurface(app->renderer, app->surface[1]);
    if (app->texture[1] == NULL) {
        printf("null medium :(");
        exit(-1);
    }


    //    app->font = TTF_OpenFont("../fonts/ariblk.ttf", 25);
    TTF_SizeText(app->font, "Let's go!", &text_width, &text_height);
    SDL_Rect textbox_white = {(SCREEN_WIDTH - text_width) / 2, 430, text_width, text_height};
    app->surface[2] = TTF_RenderText_Shaded(app->font, "Let's go!", text_color_white, background_color);
    app->texture[2] = SDL_CreateTextureFromSurface(app->renderer, app->surface[2]);
    if (app->texture[2] == NULL) {
        printf("null white :(");
        exit(-1);
    }


    SDL_Event event;
    SDL_Rect name_rect;
    char text[100] = {'\0'};
    char ch[3] = {'\0'};
    bool shift = false;
    bool lock = false;
    bool quit = false;
    bool render = true;
    while (!quit) {
        if (render) {

            SDL_RenderCopy(app->renderer, image_texture, NULL, NULL);

            roundedRectangleRGBA(app->renderer, 200, 250, 800, 300, 10, 0, 0, 0, 255);

            SDL_RenderCopy(app->renderer, app->texture[0], NULL, &textbox_huge);
            SDL_RenderCopy(app->renderer, app->texture[1], NULL, &textbox_medium);

            if (strlen(text) > 0) {
                TTF_SizeText(app->font, text, &text_width, &text_height);
                name_rect.x = (SCREEN_WIDTH - text_width) / 2;
                name_rect.y = 250;
                name_rect.w = text_width;
                name_rect.h = text_height;
                app->surface[3] = TTF_RenderText_Shaded(app->font, text, text_color, background_color);
                app->texture[3] = SDL_CreateTextureFromSurface(app->renderer, app->surface[3]);
                SDL_RenderCopy(app->renderer, app->texture[3], NULL, &name_rect);
            }

            roundedBoxColor(app->renderer, 400, 420, 600, 480, 20, 0xeeb03c30);
            SDL_RenderCopy(app->renderer, app->texture[2], NULL, &textbox_white);

            stringRGBA(app->renderer, 400, SCREEN_HEIGHT - 20, "made by Sayyed Ali Tayyeb", 255, 255, 255, 255);

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


    //reading and writing on users files
    char str[100];
    int coin = 0;
    int number_of_users = 0;
    bool end = false;
    FILE *file = fopen("../data/users.dat", "rb+");
    if (file == NULL) {
        printf("Err: can not open users file");
        number_of_users = 0;
        fclose(file);
        file = fopen("../data/users.dat", "wb+");
    }else{
        fread(&number_of_users, sizeof(number_of_users), 1, file);
    }
    for (int i = 0; i < number_of_users; i++) {
        fread(str, 1, 100, file);
        fread(&coin, sizeof(int), 1, file);
        printf("\n%s\n", str);
        if (strcmp(str, text) == 0) {
            strcpy(app->user, text);
            app->coin = coin;
            end = true;
            break;
        }
    }
    if (!end) {
        strcpy(app->user, text);
        app->coin = 100;
        fseek(file, sizeof(int) + (number_of_users * (100 + sizeof(app->coin))), SEEK_SET);
        fwrite(text, 1, 100, file);
        fwrite(&app->coin, sizeof(int), 1, file);
        number_of_users++;
        fseek(file, 0, SEEK_SET);
        fwrite(&number_of_users, sizeof(number_of_users), 1, file);
    }
    fclose(file);
    return 1;
}


bool select_map(APP *app, POINT *array, int *number_of_points) {

    //background -> app->surface[15]

    SDL_Color text_color_white = {255, 255, 255, 255};
    SDL_Color background_color = {0, 0, 0, 1};
    int text_width, text_height;


    TTF_SizeText(app->font, "next map", &text_width, &text_height);
    SDL_Rect rect1 = {(SCREEN_WIDTH - text_width) / 2 + 300, SCREEN_HEIGHT - 100, text_width, text_height};
    app->surface[4] = TTF_RenderText_Shaded(app->font, "next map", text_color_white, background_color);
    app->texture[4] = SDL_CreateTextureFromSurface(app->renderer, app->surface[4]);


    TTF_SizeText(app->font, "previous map", &text_width, &text_height);
    SDL_Rect rect2 = {(SCREEN_WIDTH - text_width) / 2 - 320, SCREEN_HEIGHT - 100, text_width, text_height};
    app->surface[5] = TTF_RenderText_Shaded(app->font, "previous map", text_color_white, background_color);
    app->texture[5] = SDL_CreateTextureFromSurface(app->renderer, app->surface[5]);


    TTF_SizeText(app->font, "generate random map", &text_width, &text_height);
    SDL_Rect rect3 = {(SCREEN_WIDTH - text_width) / 2, SCREEN_HEIGHT - 100, text_width, text_height};
    app->surface[6] = TTF_RenderText_Shaded(app->font, "generate random map", text_color_white, background_color);
    app->texture[6] = SDL_CreateTextureFromSurface(app->renderer, app->surface[6]);


    TTF_SizeText(app->font, "START", &text_width, &text_height);
    SDL_Rect rect4 = {(SCREEN_WIDTH - text_width) / 2, SCREEN_HEIGHT - 180, text_width, text_height};
    app->surface[7] = TTF_RenderText_Shaded(app->font, "START", text_color_white, background_color);
    app->texture[7] = SDL_CreateTextureFromSurface(app->renderer, app->surface[7]);


    SDL_Event event;

    char first_path[] = "../data/pre-made-maps/";
    char str[20] = {'\0'};
    int map_number = 1;
    bool random_generated = false;
    bool free_and_exit = false;
    bool render = true;
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
            SDL_RenderCopy(app->renderer, app->texture[15], NULL, NULL);

            draw_the_map(app, array, *number_of_points);

            roundedBoxColor(app->renderer, 70, SCREEN_HEIGHT - 110, 290, SCREEN_HEIGHT - 50, 5, 0xddb03c30);
            roundedBoxColor(app->renderer, 330, SCREEN_HEIGHT - 110, 670, SCREEN_HEIGHT - 50, 5, 0xddb03c30);
            roundedBoxColor(app->renderer, 720, SCREEN_HEIGHT - 110, 880, SCREEN_HEIGHT - 50, 5, 0xddb03c30);
            roundedBoxColor(app->renderer, (SCREEN_WIDTH - text_width) / 2 - 20, SCREEN_HEIGHT - 190,
                            (SCREEN_WIDTH + text_width) / 2 + 20, SCREEN_HEIGHT - 130, 5, 0xddb03c30);

            SDL_RenderCopy(app->renderer, app->texture[4], NULL, &rect1);
            SDL_RenderCopy(app->renderer, app->texture[5], NULL, &rect2);
            SDL_RenderCopy(app->renderer, app->texture[6], NULL, &rect3);
            SDL_RenderCopy(app->renderer, app->texture[7], NULL, &rect4);

            SDL_RenderPresent(app->renderer);

            random_generated = false;
            render = false;
        }

        SDL_PollEvent(&event);
        switch (event.type) {
            case SDL_QUIT:
                return 0;

            case SDL_MOUSEBUTTONUP:
                lock = false;
                break;

            case SDL_MOUSEBUTTONDOWN:
                if (!lock && event.button.button == SDL_BUTTON_LEFT) {
                    lock = true;
                    int x_mouse, y_mouse;
                    SDL_GetMouseState(&x_mouse, &y_mouse);
                    if (SCREEN_HEIGHT - 110 < y_mouse && y_mouse < SCREEN_HEIGHT - 50) {

                        //previous button
                        if (70 < x_mouse && x_mouse < 290 && map_number > 1) {
                            map_number--;
                            render = 1;


                            //generate button
                        } else if (330 < x_mouse && x_mouse < 670) {
                            *number_of_points = generate_random_map(array, app);
                            random_generated = true;
                            render = 1;


                            //next button
                        } else if (720 < x_mouse && x_mouse < 880) {
                            map_number++;
                            render = 1;
                        }

                        //start button
                    } else if (SCREEN_HEIGHT - 190 < y_mouse &&
                               y_mouse < SCREEN_HEIGHT - 130 &&
                               (SCREEN_WIDTH - text_width) / 2 - 20 < x_mouse &&
                               x_mouse < (SCREEN_WIDTH + text_width) / 2 - 20) {
                        quit = true;
                        break;
                    }
                }
                break;


            case SDL_KEYDOWN:
                if (event.key.keysym.sym == SDLK_t) {
                    do {
                        *number_of_points = generate_random_map(array, app);
                    } while (*number_of_points < 12);
                    for (int i = 0; i < *number_of_points - 1; i++) {
                        array[i].ownership = 1;
                        array[i].value = 10;
                    }
                    array[*number_of_points-1].ownership = 2;
                    array[*number_of_points-1].value = 10;
                    random_generated = true;
                    render = 1;
                }
                break;
        }
    }
    return 1;
}


bool
present_second_screen(APP *app, SDL_Texture *image_texture, POINT *array, int *number_of_points, SOLDIER **soldiers,
                      POT *pot) {

    SDL_RenderClear(app->renderer);

    SDL_Color text_color_white = {255, 255, 255, 255};
    SDL_Color background_color = {0, 0, 0, 1};
    int text_width, text_height;


    TTF_SizeText(app->font, "NEW GAME", &text_width, &text_height);
    SDL_Rect rect1 = {(SCREEN_WIDTH - text_width) / 2, 205, text_width, text_height};
    if (app->first_run) {
        app->surface[8] = TTF_RenderText_Shaded(app->font, "NEW GAME", text_color_white, background_color);
        app->texture[8] = SDL_CreateTextureFromSurface(app->renderer, app->surface[8]);
    }

    TTF_SizeText(app->font, "LOAD GAME", &text_width, &text_height);
    SDL_Rect rect2 = {(SCREEN_WIDTH - text_width) / 2, 305, text_width, text_height};
    if (app->first_run) {
        app->surface[9] = TTF_RenderText_Shaded(app->font, "LOAD GAME", text_color_white, background_color);
        app->texture[9] = SDL_CreateTextureFromSurface(app->renderer, app->surface[9]);
    }

    TTF_SizeText(app->font, "SCORES", &text_width, &text_height);
    SDL_Rect rect3 = {(SCREEN_WIDTH - text_width) / 2, 405, text_width, text_height};
    if (app->first_run) {
        app->surface[10] = TTF_RenderText_Shaded(app->font, "SCORES", text_color_white, background_color);
        app->texture[10] = SDL_CreateTextureFromSurface(app->renderer, app->surface[10]);
    }

    SDL_Event event;
    bool render = true;
    bool quit = false;
    while (!quit) {

        if (render) {
            //background
            SDL_RenderCopy(app->renderer, image_texture, NULL, NULL);

            //buttons
            roundedBoxColor(app->renderer, 350, 180, SCREEN_WIDTH - 350, 270, 5, 0xddb03c30);
            roundedBoxColor(app->renderer, 350, 280, SCREEN_WIDTH - 350, 370, 5, 0xddb03c30);
            roundedBoxColor(app->renderer, 350, 380, SCREEN_WIDTH - 350, 470, 5, 0xddb03c30);
            SDL_RenderCopy(app->renderer, app->texture[8], NULL, &rect1);
            SDL_RenderCopy(app->renderer, app->texture[9], NULL, &rect2);
            SDL_RenderCopy(app->renderer, app->texture[10], NULL, &rect3);

            //display coins
            roundedBoxColor(app->renderer, 400, 50, SCREEN_WIDTH - 400, 100, 5, 0xddffffff);
            stringRGBA(app->renderer, 465, 70, "coin:", 0, 0, 0, 255);
            char str[5] = {'\0'};;
            sprintf(str, "%d", app->coin);
            stringRGBA(app->renderer, 515, 70, str, 0, 0, 0, 255);

            SDL_RenderPresent(app->renderer);
            render = false;
        }

        SDL_Delay(10);
        SDL_PollEvent(&event);

        switch (event.type) {
            case SDL_QUIT:
                return 0;
                break;

            case SDL_MOUSEBUTTONDOWN:
                if (event.button.button == SDL_BUTTON_LEFT) {
                    int x_mouse, y_mouse;
                    SDL_GetMouseState(&x_mouse, &y_mouse);
                    if (350 < x_mouse && x_mouse < SCREEN_WIDTH - 350) {

                        //new game button
                        if (180 < y_mouse && y_mouse < 270) {
                            if (select_map(app, array, number_of_points)) {
                                quit = true;
                                break;
                            } else {
                                return 0;
                            }

                            //load game button
                        } else if (280 < y_mouse && y_mouse < 370) {
                            FILE *file = fopen("../data/data.dat", "rb");
                            if (file == NULL)
                                break;
                            fread(&pot->player1_pot_number, sizeof(int), 1, file);
                            if(feof(file)){
                                break;
                            }
                            fread(&pot->player2_pot_number, sizeof(int), 1, file);
                            fread(&pot->player1_counter, sizeof(int), 1, file);
                            fread(&pot->player2_counter, sizeof(int), 1, file);
                            fread(number_of_points, sizeof(*number_of_points), 1, file);
                            fread(array, sizeof(array[0]), *number_of_points, file);
                            int count = 0;
                            for (int i = 0; i < ATTACK_LIMIT; i++) {
                                fread(&count, sizeof(count), 1, file);
                                if (feof(file)) {
                                    break;
                                }
                                soldiers[i] = calloc(count, sizeof(SOLDIER));
                                fread(soldiers[i], sizeof(SOLDIER), count, file);
                            }
                            fclose(file);
                            quit = true;
                        }

                        //scores button
                        if (380 < y_mouse && y_mouse < 470) {
                            if (show_scores(app, image_texture))
                                render = true;
                            else
                                return 0;
                        }
                    }
                }
                break;
        }
    }
    return 1;
}

