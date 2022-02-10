#include "globals.h"
#include "init.h"
#include "menu.h"
#include "map.h"
//#include "../sdl2/sdl2-mixer-include/SDL2/SDL_mixer.h"

#ifdef main
#undef main
#endif


int main(int argc, char *argv[]) {
    bool I_win;
    bool AI_win;
    POT pot;
    SDL_Rect pot_rect;
    int AI_counter = 0;
    int pot_number = -1;
    int starting_point = -1;
    int potion_making_counter = 0;
    int soldier_making_counter = 0;
    int potion_making_timeout = random_between(700, 1800);
    srand((int) sin((double) time(0)) * time(0));


    APP *app = calloc(1, sizeof(APP));
    if (!init(app)) {
        die(app);
    }

    app->first_run = true;


    //load background image (sea and beach)
    app->surface[15] = IMG_Load("../media/sea3.jpg");
    app->texture[15] = SDL_CreateTextureFromSurface(app->renderer, app->surface[15]);
    if (app->texture[15] == NULL) {
        printf("can not load back ground image :( ");
        die(app);
    }


    int number_of_points;
    POINT array[15] = {0};
    SOLDIER *soldiers[ATTACK_LIMIT] = {NULL};


    //load island image
    app->surface[16] = IMG_Load("../media/island.jpg");
    app->texture[16] = SDL_CreateTextureFromSurface(app->renderer, app->surface[16]);
    if (app->texture[16] == NULL) {
        printf("can not load first image :( ");
        die(app);
    }


    //load potions
    app->surface[11] = IMG_Load("../media/purple.png");
    app->texture[11] = SDL_CreateTextureFromSurface(app->renderer, app->surface[11]);
    app->surface[12] = IMG_Load("../media/sam.png");
    app->texture[12] = SDL_CreateTextureFromSurface(app->renderer, app->surface[12]);
    app->surface[13] = IMG_Load("../media/red.png");
    app->texture[13] = SDL_CreateTextureFromSurface(app->renderer, app->surface[13]);
    app->surface[14] = IMG_Load("../media/yellow.png");
    app->texture[14] = SDL_CreateTextureFromSurface(app->renderer, app->surface[14]);


    if (!present_first_screen(app, app->texture[16]))
        die(app);

    bool quit = false;
    while (!quit) {

        //at the start there are not an active potion
        pot.player1_pot_number = pot.player2_pot_number = -1;

        if (!present_second_screen(app, app->texture[16], array, &number_of_points, soldiers, &pot)) {
            die(app);
        }

        app->first_run = false;
        char coin_str[5];
        sprintf(coin_str, "%d", app->coin);

        SDL_Event event;

        while (!quit) {

            SDL_PollEvent(&event);

            switch (event.type) {
                case SDL_QUIT:
                    quit = true;
                    break;

                case SDL_MOUSEBUTTONDOWN:
                    if (event.button.button == SDL_BUTTON_LEFT && starting_point == -1) {
                        int point_number = witch_point(event.button.x, event.button.y, array, number_of_points);
                        if (array[point_number].ownership == 1 /*|| array[point_number].ownership == 2 /**/) {
                            starting_point = point_number;
                        }
                    }
                    break;

                case SDL_MOUSEBUTTONUP:
                    if (event.button.button == SDL_BUTTON_LEFT && starting_point != -1) {
                        int ending_point = witch_point(event.button.x, event.button.y, array, number_of_points);
                        if (ending_point != -1 && ending_point != starting_point) {
                            attack(app, array, soldiers, starting_point, ending_point, &pot);
                        }
                    }
                    starting_point = -1;
                    break;

                case SDL_KEYDOWN:
                    //saving a game
                    if (event.key.keysym.sym == SDLK_s) {
                        FILE *file = fopen("../data/data.dat", "wb");
                        fwrite(&pot.player1_pot_number, sizeof(int), 1, file);
                        fwrite(&pot.player2_pot_number, sizeof(int), 1, file);
                        fwrite(&pot.player1_counter, sizeof(int), 1, file);
                        fwrite(&pot.player2_counter, sizeof(int), 1, file);
                        fwrite(&number_of_points, sizeof(number_of_points), 1, file);
                        fwrite(array, sizeof(array[0]), number_of_points, file);
                        for (int i = 0; i < ATTACK_LIMIT; i++) {
                            if (soldiers[i] != NULL) {
                                fwrite(&soldiers[i][0].number_of_companions, sizeof(int), 1, file);
                                fwrite(soldiers[i], sizeof(SOLDIER), soldiers[i][0].number_of_companions, file);
                            }
                        }
                        fclose(file);
                    }
                    break;
            }


            //AI
            AI_counter++;
            if (AI_counter > 700) {
                AI_counter = 0;
                AI(app, array, number_of_points, soldiers, &pot);
            }

            //soldier making progress
            soldier_making_counter++;
            if (soldier_making_counter > 100) {
                soldier_making_counter = 0;
                make_soldier(array, number_of_points, &pot);
            }

            //timer for disappearing the potion on screen
            if (potion_making_counter > 400) {
                pot_number = -1;
            }

            //timer for deactivating the potions
            if (pot.player1_counter > 0)
                pot.player1_counter -= 1;
            else
                pot.player1_pot_number = -1;
            if (pot.player2_counter > 0)
                pot.player2_counter -= 1;
            else
                pot.player2_pot_number = -1;




            ////////////////render the screen////////////////

            //render background image
            SDL_RenderClear(app->renderer);
            SDL_RenderCopy(app->renderer, app->texture[15], NULL, NULL);

            //render the map
            draw_the_map(app, array, number_of_points);

            //render the potions
            potion_making_counter++;
            if (pot_number != -1) {
                switch (pot_number) {
                    case 1:
                        SDL_RenderCopy(app->renderer, app->texture[11], NULL, &pot_rect);
                        break;
                    case 2:
                        SDL_RenderCopy(app->renderer, app->texture[12], NULL, &pot_rect);
                        break;
                    case 3:
                        SDL_RenderCopy(app->renderer, app->texture[13], NULL, &pot_rect);
                        break;
                    case 4:
                        SDL_RenderCopy(app->renderer, app->texture[14], NULL, &pot_rect);
                        break;
                }
            }

            //potion making
            if (potion_making_counter > potion_making_timeout) {
                potion_making_counter = 0;
                potion_making_timeout = random_between(800, 1800);
                pot_rect.w = pot_rect.h = 50;
                quit = false;
                while (!quit) {
                    int a = random_between(0, number_of_points - 1);
                    int b = random_between(0, number_of_points - 1) % (number_of_points - 1) + 1;
                    pot_rect.x = (array[a].x + array[b].x) / 2 - 25;
                    pot_rect.y = (array[a].y + array[b].y) / 2 - 25;
                    POINT pot_location = {.x = pot_rect.x + 25, .y = pot_rect.y + 25, .r = 5};
                    quit = true;
                    for (int i = 0; i < number_of_points; i++) {
                        if (bad_distance(pot_location, array[i])) {
                            quit = false;
                            break;
                        }
                    }
                }
                quit = false;
                pot_number = random_between(1, 4);
            }

            render_active_potions(app, &pot, app->texture[11], app->texture[12], app->texture[13], app->texture[14]);

            //render soldiers
            move(app, array, soldiers, pot_rect, &pot_number, &pot);

            //render arrow
            if (starting_point != -1) {
                int x_mouse, y_mouse;
                SDL_GetMouseState(&x_mouse, &y_mouse);
                arrow(app, array[starting_point].x, array[starting_point].y, x_mouse, y_mouse, 15, 0xFFaaabaa);
            }

            stringRGBA(app->renderer, 400, SCREEN_HEIGHT - 20, "press s to save the game", 0, 0, 0, 255);

            SDL_RenderPresent(app->renderer);
            SDL_Delay(5);


            if (who_won(array, number_of_points, soldiers) == 1) {
                printf("I won!");
                app->coin += 10;
                break;
            }

            if (who_won(array, number_of_points, soldiers) == 2) {
                printf("You lost!");
                app->coin -= 10;
                break;
            }
        }

        //save in file
        int number_of_users = 0;
        int coin;
        char str[100];
        FILE *file = fopen("../data/users.dat", "rb+");
        fread(&number_of_users, sizeof(number_of_users), 1, file);
        for (int i = 0; i < number_of_users; i++) {
            fread(str, 1, 100, file);
            if (strcmp(str, app->user) == 0) {
                fseek(file, sizeof(int) + 100 + (i * (100 + sizeof(int))), SEEK_SET);
                fwrite(&app->coin, sizeof(int), 1, file);
                break;
            }
            fread(&coin, sizeof(int), 1, file);
        }

        //free soldiers
        for (int i = 0; i < ATTACK_LIMIT; i++) {
            if (soldiers[i] == NULL)
                continue;
            free(soldiers[i]);
            soldiers[i] = NULL;
        }
    }
    //free memory and exit
    die(app);
}

