#ifndef GAME_OVER_H
#define GAME_OVER_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "scores.h"

void show_game_over(SDL_Renderer *renderer, TTF_Font *font, Scores scores, int game_mode, int player_turn);
int handle_game_over_events(SDL_Event *event, int *selected_option);

#endif // GAME_OVER_H