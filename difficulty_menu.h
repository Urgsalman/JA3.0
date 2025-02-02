#ifndef DIFFICULTY_MENU_H
#define DIFFICULTY_MENU_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

void show_difficulty_menu(SDL_Renderer *renderer, TTF_Font *font, int selected_option);
int handle_difficulty_menu_events(SDL_Event *event, int *selected_option);

#endif // DIFFICULTY_MENU_H