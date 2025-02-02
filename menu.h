#ifndef MENU_H
#define MENU_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

typedef struct {
    float scale;
    float rotation;
    float hover_effect;
} MenuAnimation;

void init_menu_animations();
void update_menu_animations(int selected_option);
void draw_title(SDL_Renderer *renderer, TTF_Font *font);
void show_menu(SDL_Renderer *renderer, TTF_Font *font, int selected_option);
int handle_menu_events(SDL_Event *event, int *selected_option);

#endif