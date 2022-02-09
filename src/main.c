#include "globals.h"
#include "init.h"
#include "menu.h"
#include "map.h"


#ifdef main
#undef main
#endif


void AI(APP *app, POINT *array, int number_of_points, SOLDIER **soldiers, POT *pot) {
    int max_point = 0;
    int max_value = 0;
    for (int i = 0; i < number_of_points; i++) {
        if (array[i].ownership == 2 && array[i].value > max_value) {
            max_value = array[i].value;
            max_point = i;
        }
    }
    int min_point = 0;
    int min_value = 1000;
    for (int i = 0; i < number_of_points; i++) {
        if (array[i].ownership != 2 && array[i].value < min_value) {
            min_value = array[i].value;
            min_point = i;
        }
    }
    attack(app, array, soldiers, max_point, min_point, pot);
}


int who_won(POINT *array, int number_of_points, SOLDIER **soldiers) {
    int I_win = true;
    int AI_win = true;
    for (int i = 0; i < number_of_points; i++) {
        if (array[i].ownership == 1)
            AI_win = false;

        if (array[i].ownership == 2)
            I_win = false;
    }
    for (int i = 0; i < ATTACK_LIMIT; i++) {
        if (soldiers[i] == NULL)
            continue;

        if (soldiers[i]->ownership == 1)
            AI_win = false;

        if (soldiers[i]->ownership == 2)
            I_win = false;
    }
    if (I_win)
        return 1;

    if(AI_win)
        return 2;

    return -1;
}


int main(int argc, char *argv[]) {
    bool I_win;
    bool AI_win;
    SDL_Rect pot_rect;
    int pot_number = -1;
    int starting_point = -1;
    int soldier_making_counter = 0;
    int potion_making_counter = 0;
    int AI_counter = 0;
    int potion_making_timeout = random_between(700, 1800);
    srand((int) sin((double) time(0)) * time(0));


    APP *app = calloc(1, sizeof(APP));
    if (!init(app)) {
        die(app);
    }

    //load background image
    SDL_Surface *_image = IMG_Load("../media/sea3.jpg");
    SDL_Delay(5);
    SDL_Texture *_image_texture = SDL_CreateTextureFromSurface(app->renderer, _image);
    if (_image_texture == NULL) {
        printf("can not load back ground image :( ");
        die(app);
    }


    int number_of_points;
    POINT array[15] = {0};
    SOLDIER *soldiers[ATTACK_LIMIT] = {NULL};


    //load island image
    SDL_Surface *image = IMG_Load("../media/island.jpg");
    SDL_Texture *image_texture = SDL_CreateTextureFromSurface(app->renderer, image);
    if (image_texture == NULL) {
        printf("can not load first image :( ");
        SDL_FreeSurface(_image);
        SDL_DestroyTexture(_image_texture);
        die(app);
    }

    if (!present_first_screen(app, image_texture) ||/**/
        !present_second_screen(app, image_texture, array, &number_of_points, soldiers)) {
        SDL_FreeSurface(_image);
        SDL_DestroyTexture(_image_texture);
        SDL_FreeSurface(image);
        SDL_DestroyTexture(image_texture);
        die(app);
    }

    SDL_FreeSurface(image);
    SDL_DestroyTexture(image_texture);

    //load potions
    SDL_Surface *pot1_surf = IMG_Load("../media/purple.png");
    SDL_Texture *pot1_tex = SDL_CreateTextureFromSurface(app->renderer, pot1_surf);
    SDL_Surface *pot2_surf = IMG_Load("../media/sam.png");
    SDL_Texture *pot2_tex = SDL_CreateTextureFromSurface(app->renderer, pot2_surf);
    SDL_Surface *pot3_surf = IMG_Load("../media/red.png");
    SDL_Texture *pot3_tex = SDL_CreateTextureFromSurface(app->renderer, pot3_surf);
    SDL_Surface *pot4_surf = IMG_Load("../media/yellow.png");
    SDL_Texture *pot4_tex = SDL_CreateTextureFromSurface(app->renderer, pot4_surf);



    ///////////////////////////////////////////////////////
    int a = random_between(0, number_of_points - 1);
    int b = random_between(0, number_of_points - 1) % (number_of_points - 1) + 1;
    pot_rect.x = (array[a].x + array[b].x) / 2 - 10;
    pot_rect.y = (array[a].y + array[b].y) / 2 - 10;
    pot_rect.w = pot_rect.h = 50;
    ////////////////////////////////////////////////////////



    POT pot = {.player1_pot_number = -1, .player2_pot_number = -1};


    SDL_Event event;
    bool quit = false;
    while (!quit) {

        SDL_PollEvent(&event);

        switch (event.type) {
            case SDL_QUIT:
                quit = true;
                break;

            case SDL_MOUSEBUTTONDOWN:
                if (event.button.button == SDL_BUTTON_LEFT && starting_point == -1) {
                    int point_number = witch_point(event.button.x, event.button.y, array, number_of_points);
                    if (array[point_number].ownership == 1 || array[point_number].ownership == 2) {
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


        switch (who_won(array, number_of_points, soldiers)) {
            case 1:
                //TODO you win
                printf("I won!");
                break;
            case 2:
                //TODO computer won!
                printf("You lost!");
                break;
            default:
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
        SDL_RenderCopy(app->renderer, _image_texture, NULL, NULL);

        //render the map
        draw_the_map(app, array, number_of_points);

        //render the potions
        potion_making_counter++;
        if (pot_number != -1) {
            switch (pot_number) {
                case 1:
                    SDL_RenderCopy(app->renderer, pot1_tex, NULL, &pot_rect);
                    break;
                case 2:
                    SDL_RenderCopy(app->renderer, pot2_tex, NULL, &pot_rect);
                    break;
                case 3:
                    SDL_RenderCopy(app->renderer, pot3_tex, NULL, &pot_rect);
                    break;
                case 4:
                    SDL_RenderCopy(app->renderer, pot4_tex, NULL, &pot_rect);
                    break;
            }
        }
        //potion making
        if (potion_making_counter > potion_making_timeout) {
            potion_making_counter = 0;
            potion_making_timeout = random_between(1000, 2000);
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

        render_active_potions(app, &pot, pot1_tex, pot2_tex, pot3_tex, pot4_tex);

        //render soldiers
        move(app, array, soldiers, pot_rect, &pot_number, &pot);

        //render arrow
        if (starting_point != -1) {
            int x_mouse, y_mouse;
            SDL_GetMouseState(&x_mouse, &y_mouse);
            arrow(app, array[starting_point].x, array[starting_point].y, x_mouse, y_mouse, 15, 0xFFaaabaa);
        }

        SDL_RenderPresent(app->renderer);
        SDL_Delay(5);
    }

    //free memory and exit
    SDL_FreeSurface(_image);
    SDL_DestroyTexture(_image_texture);
    SDL_FreeSurface(pot1_surf);
    SDL_FreeSurface(pot2_surf);
    SDL_FreeSurface(pot3_surf);
//    SDL_FreeSurface(pot4_surf);
    die(app);
}

