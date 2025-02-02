#include "game.h"
#include "graphics.h"
#include "ai.h"
#include "scores.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <math.h>

static GameState gameState = {NULL, 0, 0, 0, 0};

void init_game_if_needed() {
    if (!gameState.initialized) {
        gameState.animations = (StickAnimation*)malloc(sizeof(StickAnimation) * MAX_STICKS);
        init_stick_animations(gameState.animations, MAX_STICKS);
        gameState.initialized = 1;
        gameState.last_move_time = SDL_GetTicks();
        gameState.move_made = 0;
        gameState.winning_line_shown = 0;
    }
}

void cleanup_game() {
    if (gameState.initialized) {
        free(gameState.animations);
        gameState.animations = NULL;
        gameState.initialized = 0;
    }
}

int check_winner(int total_sticks) {
    return total_sticks <= 0;
}

void update_game_state(int *total_sticks, int *player_turn, int sticks_to_remove, 
                      Scores *scores, int game_mode) {
    if (sticks_to_remove > 0 && sticks_to_remove <= 3 && sticks_to_remove <= *total_sticks) {
        *total_sticks -= sticks_to_remove;
        *player_turn = !(*player_turn);
        gameState.move_made = 1;
        gameState.last_move_time = SDL_GetTicks();
        
        if (check_winner(*total_sticks)) {
            if (*player_turn == 0) {
                scores->score_joueur2++;
            } else {
                scores->score_joueur1++;
            }
            sauvegarder_scores(*scores);
            gameState.winning_line_shown = 0;
        }
    }
}

void handle_ai_turn(int *total_sticks, int *player_turn, int ai_difficulty) {
    Uint32 current_time = SDL_GetTicks();
    if (current_time - gameState.last_move_time > 1000) { // 1 second delay
        int sticks_to_remove = 0;
        switch (ai_difficulty) {
            case 0: sticks_to_remove = ai_easy(*total_sticks); break;
            case 1: sticks_to_remove = ai_medium(*total_sticks); break;
            case 2: sticks_to_remove = ai_hard(*total_sticks); break;
        }
        
        if (sticks_to_remove > 0 && sticks_to_remove <= 3 && sticks_to_remove <= *total_sticks) {
            *total_sticks -= sticks_to_remove;
            *player_turn = !(*player_turn);
            gameState.last_move_time = current_time;
            gameState.move_made = 1;
        }
    }
}

int handle_game_input(SDL_Event *event, int *total_sticks, int *player_turn, 
                     Scores *scores, int game_mode, int ai_difficulty) {
    if (event->type == SDL_MOUSEBUTTONDOWN) {
        int x = event->button.x;
        int y = event->button.y;
        int sticks_to_remove = 0;

        if (y >= 500 && y <= 550) {
            if (x >= 100 && x <= 250) sticks_to_remove = 1;
            else if (x >= 325 && x <= 475) sticks_to_remove = 2;
            else if (x >= 550 && x <= 700) sticks_to_remove = 3;
        }

        if (sticks_to_remove > 0) {
            update_game_state(total_sticks, player_turn, sticks_to_remove, scores, game_mode);
            return gameState.move_made;
        }
    }
    
    if (event->type == SDL_KEYDOWN) {
        int sticks_to_remove = 0;
        switch (event->key.keysym.sym) {
            case SDLK_1: case SDLK_KP_1: sticks_to_remove = 1; break;
            case SDLK_2: case SDLK_KP_2: sticks_to_remove = 2; break;
            case SDLK_3: case SDLK_KP_3: sticks_to_remove = 3; break;
        }
        
        if (sticks_to_remove > 0) {
            update_game_state(total_sticks, player_turn, sticks_to_remove, scores, game_mode);
            return gameState.move_made;
        }
    }
    
    return 0;
}

void show_winner_message(SDL_Renderer *renderer, TTF_Font *font, int player_turn, 
                        int game_mode, Scores *scores) {
    char message[100];
    if (game_mode == 1 && player_turn == 0) {
        sprintf(message, "Computer wins!");
    } else {
        sprintf(message, "Player %d wins!", player_turn == 0 ? 2 : 1);
    }

    SDL_Color color = {255, 0, 0, 255};
    render_text(renderer, font, message, 400, 300, color, 1);

    char scores_text[100];
    sprintf(scores_text, "Scores - P1: %d | P2: %d", scores->score_joueur1, scores->score_joueur2);
    render_text(renderer, font, scores_text, 400, 350, color, 1);
}

void play_game(SDL_Renderer *renderer, TTF_Font *font, int *total_sticks, 
               int *player_turn, Scores *scores, int game_mode, int ai_difficulty) {
    init_game_if_needed();
    gameState.move_made = 0;
    
    update_stick_animations(gameState.animations, *total_sticks);
    
    draw_background(renderer);
    draw_sticks(renderer, *total_sticks, gameState.animations);
    
    // Draw game state information
    char message[100];
    SDL_Color color = {0, 0, 139, 255};
    
    if (game_mode == 1 && *player_turn == 1) {
        sprintf(message, "Computer's Turn");
    } else {
        sprintf(message, "Player %d's Turn", *player_turn + 1);
    }
    
    render_text(renderer, font, message, 400, 50, color, 1);
    
    // Draw remaining sticks counter
    char sticks_text[50];
    sprintf(sticks_text, "Remaining Sticks: %d", *total_sticks);
    SDL_Color sticks_color = {139, 69, 19, 255};
    render_text(renderer, font, sticks_text, 400, 100, sticks_color, 1);
    
    // Draw buttons with hover effect
    int mouse_x, mouse_y;
    SDL_GetMouseState(&mouse_x, &mouse_y);
    
    int button_selected = -1;
    if (mouse_y >= 500 && mouse_y <= 550) {
        if (mouse_x >= 100 && mouse_x <= 250) button_selected = 0;
        else if (mouse_x >= 325 && mouse_x <= 475) button_selected = 1;
        else if (mouse_x >= 550 && mouse_x <= 700) button_selected = 2;
    }
    
    draw_button(renderer, font, "Take 1", 100, 500, 150, 50, button_selected == 0);
    draw_button(renderer, font, "Take 2", 325, 500, 150, 50, button_selected == 1);
    draw_button(renderer, font, "Take 3", 550, 500, 150, 50, button_selected == 2);
    
    // Handle AI turn if it's computer's turn
    if (game_mode == 1 && *player_turn == 1 && !check_winner(*total_sticks)) {
        handle_ai_turn(total_sticks, player_turn, ai_difficulty);
    }
    
    // Show winner message if game is over
    if (check_winner(*total_sticks) && !gameState.winning_line_shown) {
        show_winner_message(renderer, font, *player_turn, game_mode, scores);
        if (!gameState.winning_line_shown) {
            draw_winning_line(renderer, 0, 300, 800, 300); // Removed extra argument
            gameState.winning_line_shown = 1;
        }
    }
}