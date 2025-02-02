#include "game_over.h"
#include "graphics.h"

void show_game_over(SDL_Renderer *renderer, TTF_Font *font, Scores scores, int game_mode, int player_turn) {
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);

    char message[100];
    if (game_mode == 1 && player_turn == 0) {
        sprintf(message, "Computer wins!");
    } else if (player_turn == 0) {
        sprintf(message, "Player 2 wins!");
    } else {
        sprintf(message, "Player 1 wins!");
    }

    SDL_Color color = {255, 0, 0, 255}; // Red color for game over message
    SDL_Surface *surface = TTF_RenderText_Blended(font, message, color);
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);

    SDL_Rect message_rect = {
        400 - surface->w / 2,
        200,
        surface->w,
        surface->h
    };
    SDL_RenderCopy(renderer, texture, NULL, &message_rect);

    // Draw scores
    char scores_text[100];
    sprintf(scores_text, "Player 1: %d | Player 2: %d", scores.score_joueur1, scores.score_joueur2);
    SDL_Surface *scores_surface = TTF_RenderText_Blended(font, scores_text, color);
    SDL_Texture *scores_texture = SDL_CreateTextureFromSurface(renderer, scores_surface);

    SDL_Rect scores_rect = {
        400 - scores_surface->w / 2,
        250,
        scores_surface->w,
        scores_surface->h
    };
    SDL_RenderCopy(renderer, scores_texture, NULL, &scores_rect);

    // Draw buttons
    draw_button(renderer, font, "Play Again", 250, 300, 300, 50, 0);
    draw_button(renderer, font, "Back to Menu", 250, 400, 300, 50, 0);

    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);
    SDL_FreeSurface(scores_surface);
    SDL_DestroyTexture(scores_texture);
}

int handle_game_over_events(SDL_Event *event, int *selected_option) {
    if (event->type == SDL_MOUSEBUTTONDOWN) {
        int x = event->button.x;
        int y = event->button.y;
        
        // Check "Play Again" button
        if (y >= 300 && y <= 350 && x >= 250 && x <= 550) {
            *selected_option = 0;
            return 1;
        }
        // Check "Back to Menu" button
        else if (y >= 400 && y <= 450 && x >= 250 && x <= 550) {
            *selected_option = 1;
            return 1;
        }
    }
    return 0;
}