#ifndef ISLAND_SOLDIER_MENU_H
#define ISLAND_SOLDIER_MENU_H



bool show_scores(APP *app,SDL_Texture* image_texture);
bool present_first_screen(APP *app, SDL_Texture *image_texture);
bool select_map(APP *app, POINT *array, int *number_of_points);
bool present_second_screen(APP *app, SDL_Texture *image_texture, POINT *array, int *number_of_points, SOLDIER **soldiers,POT*pot);


#endif //ISLAND_SOLDIER_MENU_H