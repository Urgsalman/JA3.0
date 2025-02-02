#include "credits.h"
#include "graphics.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

void show_credits(SDL_Renderer *renderer, TTF_Font *font) {
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);

    SDL_Color color = {0, 0, 0, 255}; // Black color
    const char *credits_text = "Credits:\n"
                               "- Developed by [Your Name]\n"
                               "- Uses SDL2 for GUI\n"
                               "- Matches Game Implementation\n";

    SDL_Surface *surface = TTF_RenderText_Blended_Wrapped(font, credits_text, color, 600);
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);

    SDL_Rect credits_rect = {50, 50, surface->w, surface->h};
    SDL_RenderCopy(renderer, texture, NULL, &credits_rect);

    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);
    SDL_RenderPresent(renderer);
}