#ifndef ISLAND_SOLDIER_MAP_H
#define ISLAND_SOLDIER_MAP_H



bool bad_distance(POINT first, POINT second);
int random_between(int start, int end);
int witch_point(int x, int y, POINT *array, int number_of_points);
int line_between_start_end(int x, float a, int xf, int yf);

void draw_the_map(APP *app, POINT *array, int number_of_points);
int generate_random_map(POINT array[15], APP *app);
void move_the_soldier(APP *app, POINT *array, SOLDIER **soldiers, int i, int j, POT *pot);
void move(APP *app, POINT *array, SOLDIER **soldiers, SDL_Rect pot_rect, int *pot_number, POT *pot);
void make_soldier(POINT *array, int number_of_points, POT *pot);
void arrow(APP *app, int x1, int y1, int x2, int y2, double width, Uint32 color);
void render_active_potions(APP *app, POT *pot, SDL_Texture *pot1_tex, SDL_Texture *pot2_tex, SDL_Texture *pot3_tex,SDL_Texture *pot4_tex);
void attack(APP*app, POINT*array,SOLDIER ** soldiers,int starting_point,int ending_point,POT*pot);
int who_won(POINT *array, int number_of_points, SOLDIER **soldiers);
void AI(APP *app, POINT *array, int number_of_points, SOLDIER **soldiers, POT *pot);

#endif //ISLAND_SOLDIER_MAP_H