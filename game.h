#ifndef GAME_H
#define GAME_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "scores.h"

#define MAX_STICKS 21

typedef struct {
    float x;
    float y;
    float rotation;
    float scale;
    float hover_effect;
} StickAnimation;

typedef struct {
    StickAnimation *animations;
    int initialized;
    int last_move_time;
    int move_made;
    int winning_line_shown;
} GameState;

// Core game functions
void init_game_if_needed();
void cleanup_game();
void play_game(SDL_Renderer *renderer, TTF_Font *font, int *total_sticks, 
               int *player_turn, Scores *scores, int game_mode, int ai_difficulty);
int check_winner(int total_sticks);
int handle_game_input(SDL_Event *event, int *total_sticks, int *player_turn, 
                     Scores *scores, int game_mode, int ai_difficulty);

// Helper functions
void show_winner_message(SDL_Renderer *renderer, TTF_Font *font, int player_turn, 
                        int game_mode, Scores *scores);
void update_game_state(int *total_sticks, int *player_turn, int sticks_to_remove, 
                      Scores *scores, int game_mode);
void handle_ai_turn(int *total_sticks, int *player_turn, int ai_difficulty);

#endif // GAME_H