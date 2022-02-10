#include "globals.h"
#include "map.h"


bool bad_distance(POINT first, POINT second) {
    if (((int) (sqrt(pow(first.x - second.x, 2) + pow(first.y - second.y, 2)))) < (first.r + second.r))
        return 1;
    return 0;
}


int random_between(int start, int end) {
    return start + rand() % (end - start + 1);
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
            filledCircleColor(app->renderer, array[i].x, array[i].y, array[i].r, LIGHT_BLUE);
            filledCircleColor(app->renderer, array[i].x, array[i].y, 20, DARK_BLUE);
        }
        sprintf(str, "%d", array[i].value);
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
        array[i].ownership = 0;
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
    array[5].value = 15;
    draw_the_map(app, array, number_of_points);
    return number_of_points;
}


void move_the_soldier(APP *app, POINT *array, SOLDIER **soldiers, int i, int j, POT *pot) {
    int start_x = array[soldiers[i][j].start_point].x;
    int start_y = array[soldiers[i][j].start_point].y;
    int dest_x = array[soldiers[i][j].end_point].x;
    int dest_y = array[soldiers[i][j].end_point].y;

    //calculate the line slope between start point and end point
    float a = (float) (start_y - dest_y) / (float) (dest_x - start_x);

    int speed = 2;
    if (soldiers[i][j].ownership == 1 && pot->player1_pot_number == 1 ||
        soldiers[i][j].ownership == 2 && pot->player2_pot_number == 1) {
        speed *= 2;
    }

    ///direction algorithm////
    if (start_y > dest_y) {
        if (soldiers[i][j].y > line_between_start_end(soldiers[i][j].x, a, dest_x, dest_y)) {
            soldiers[i][j].y -= speed;
        } else {
            soldiers[i][j].x += speed * ((dest_x - start_x) / abs(dest_x - start_x));
        }
    } else {
        if (soldiers[i][j].y < line_between_start_end(soldiers[i][j].x, a, dest_x, dest_y)) {
            soldiers[i][j].y += speed;
        } else {
            soldiers[i][j].x += speed * ((dest_x - start_x) / abs(dest_x - start_x));
        }
    }
}


void move(APP *app, POINT *array, SOLDIER **soldiers, SDL_Rect pot_rect, int *pot_number, POT *pot) {
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

            //freeze potion avoids thr opponent moving
            if ((pot->player1_pot_number != 4 && pot->player2_pot_number != 4) ||
                pot->player1_pot_number == 4 && soldiers[i][j].ownership == 1 ||
                pot->player2_pot_number == 4 && soldiers[i][j].ownership == 2) {


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
                move_the_soldier(app, array, soldiers, i, j, pot);

                //if a soldier reached a potion
                if (*pot_number != -1 &&
                    0 < (soldiers[i][j].x - pot_rect.x) && (soldiers[i][j].x - pot_rect.x) < 50 &&
                    0 < (soldiers[i][j].y - pot_rect.y) && (soldiers[i][j].y - pot_rect.y) < 50) {
                    if (soldiers[i][j].ownership == 1 && pot->player1_pot_number == -1) {
                        pot->player1_pot_number = *pot_number;
                        pot->player1_counter = 1300;
                        *pot_number = -1;
                    } else if (soldiers[i][j].ownership == 2 && pot->player2_pot_number == -1) {
                        pot->player2_pot_number = *pot_number;
                        pot->player2_counter = 1300;
                        *pot_number = -1;
                    }
                }

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


void make_soldier(POINT *array, int number_of_points, POT *pot) {
    for (int i = 0; i < number_of_points; i++) {
        if (array[i].ownership == 1 && array[i].value < 50) {
            array[i].value += 1;
            if (pot->player1_pot_number == 3)
                array[i].value += 2;
        }
        if (array[i].ownership == 2 && array[i].value < 50) {
            array[i].value += 1;
            if (pot->player2_pot_number == 3)
                array[i].value += 2;
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


void render_active_potions(APP *app, POT *pot, SDL_Texture *pot1_tex, SDL_Texture *pot2_tex, SDL_Texture *pot3_tex,
                           SDL_Texture *pot4_tex) {
    //render player 1 potions
    if (pot->player1_pot_number != -1) {
        SDL_Rect pot_rect_active = {SCREEN_WIDTH - 90, 10, 80, 80};
        boxRGBA(app->renderer, SCREEN_WIDTH - 100, 0, SCREEN_WIDTH, 120, 73, 240, 67, 210);
        char str[20] = {'\0'};
        switch (pot->player1_pot_number) {
            case 1:
                SDL_RenderCopy(app->renderer, pot1_tex, NULL, &pot_rect_active);
                strcpy(str, " X2 speed");
                break;
            case 2:
                SDL_RenderCopy(app->renderer, pot2_tex, NULL, &pot_rect_active);
                strcpy(str, "Anti attack");
                break;
            case 3:
                SDL_RenderCopy(app->renderer, pot3_tex, NULL, &pot_rect_active);
                strcpy(str, " X3 making");
                break;
            case 4:
                SDL_RenderCopy(app->renderer, pot4_tex, NULL, &pot_rect_active);
                strcpy(str, "  Frozen");
                break;
            default:
                break;
        }
        stringColor(app->renderer, SCREEN_WIDTH - 90, 100, str, 0xff000000);
    }

    //render player 2 potions
    if (pot->player2_pot_number != -1) {
        SDL_Rect pot_rect_active = {10, 10, 80, 80};
        boxRGBA(app->renderer, 0, 0, 100, 120, 222, 13, 13, 210);
        char str[20] = {'\0'};
        switch (pot->player2_pot_number) {
            case 1:
                SDL_RenderCopy(app->renderer, pot1_tex, NULL, &pot_rect_active);
                strcpy(str, " X2 speed");
                break;
            case 2:
                SDL_RenderCopy(app->renderer, pot2_tex, NULL, &pot_rect_active);
                strcpy(str, "Anti attack");
                break;
            case 3:
                SDL_RenderCopy(app->renderer, pot3_tex, NULL, &pot_rect_active);
                strcpy(str, " X3 making");
                break;
            case 4:
                SDL_RenderCopy(app->renderer, pot4_tex, NULL, &pot_rect_active);
                strcpy(str, "  Frozen");
                break;
            default:
                break;
        }
        stringColor(app->renderer, 5, 100, str, 0xff000000);
    }
}


void attack(APP*app, POINT*array,SOLDIER ** soldiers,int starting_point,int ending_point,POT*pot){

    //pot4 => freeze potion
    if (array[starting_point].ownership == 1 && pot->player2_pot_number == 4 ||
    array[starting_point].ownership == 2 && pot->player1_pot_number == 4) {
        starting_point = -1;
        return ;
    }

    //pot2 => anti attack potion
    if (array[starting_point].ownership == 2 && pot->player1_pot_number == 2 ||
    array[starting_point].ownership == 1 && pot->player2_pot_number == 2) {
        if (array[ending_point].ownership != array[starting_point].ownership &&
        array[ending_point].ownership != 0) {
            starting_point = -1;
            return;
        }
    }

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
    if (i == ATTACK_LIMIT)
        return;
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
    if(pot->player1_pot_number == 2){
        for (int i = 0; i < number_of_points; i++) {
            if (array[i].ownership != 1 && array[i].value < min_value) {
                min_value = array[i].value;
                min_point = i;
            }
        }
    }else{
        for (int i = 0; i < number_of_points; i++) {
            if (array[i].ownership != 2 && array[i].value < min_value) {
                min_value = array[i].value;
                min_point = i;
            }
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
