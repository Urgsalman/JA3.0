#include "rules.h"
#include "graphics.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

static float rules_time = 0;

void show_rules(SDL_Renderer *renderer, TTF_Font *font) {
    rules_time += 0.016f; // Assuming 60 FPS

    // Draw animated background
    draw_background(renderer);

    // Draw title
    SDL_Color title_color = {255, 255, 255, 255}; // White color
    const char *title = "Game Rules";

    SDL_Surface *title_surface = TTF_RenderText_Blended(font, title, title_color);
    SDL_Texture *title_texture = SDL_CreateTextureFromSurface(renderer, title_surface);

    SDL_Rect title_rect = {
        400 - title_surface->w / 2,
        50 + sin(rules_time * 3) * 5, // Add gentle floating animation
        title_surface->w,
        title_surface->h
    };

    SDL_RenderCopy(renderer, title_texture, NULL, &title_rect);

    // Draw rules text
    const char *rules_text[] = {
        "Welcome to the Matches Game!",
        "",
        "1. The game starts with 21 matches.",
        "2. Players take turns removing 1-3 matches.",
        "3. The player forced to take the last match loses.",
        "4. Think strategically to win!",
        "",
        "Press ESC to return to menu"
    };

    SDL_Color text_color = {0, 0, 139, 255}; // Dark blue

    for (int i = 0; i < 8; i++) {
        float offset = sin(rules_time * 2 + i * 0.3f) * 5; // Add wave effect

        SDL_Surface *surface = TTF_RenderText_Blended(font, rules_text[i], text_color);
        SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);

        SDL_Rect text_rect = {
            400 - surface->w / 2,
            150 + i * 50 + offset,
            surface->w,
            surface->h
        };

        SDL_RenderCopy(renderer, texture, NULL, &text_rect);

        SDL_FreeSurface(surface);
        SDL_DestroyTexture(texture);
    }

    SDL_FreeSurface(title_surface);
    SDL_DestroyTexture(title_texture);
    SDL_RenderPresent(renderer);
}