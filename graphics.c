#include "graphics.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <math.h>

void render_text(SDL_Renderer *renderer, TTF_Font *font, const char *text, 
                int x, int y, SDL_Color color, int centered) {
    if (!renderer || !font || !text) return;
    
    SDL_Surface *surface = TTF_RenderText_Blended(font, text, color);
    if (!surface) return;
    
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
    if (!texture) {
        SDL_FreeSurface(surface);
        return;
    }
    
    SDL_Rect text_rect = {
        centered ? x - surface->w / 2 : x,
        y,
        surface->w,
        surface->h
    };
    
    SDL_RenderCopy(renderer, texture, NULL, &text_rect);
    
    SDL_DestroyTexture(texture);
    SDL_FreeSurface(surface);
}

void draw_button(SDL_Renderer *renderer, TTF_Font *font, const char *text, 
                int x, int y, int w, int h, int selected) {
    if (!renderer || !font || !text) return;

    // Draw button shadow with alpha
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 64);
    SDL_Rect shadow = {x + 4, y + 4, w, h};
    SDL_RenderFillRect(renderer, &shadow);

    // Draw button gradient background
    for (int i = 0; i < h; i++) {
        int intensity = selected ? 180 - i / 2 : 220 - i / 2;
        SDL_SetRenderDrawColor(renderer, intensity, intensity, intensity, 255);
        SDL_Rect line = {x, y + i, w, 1};
        SDL_RenderFillRect(renderer, &line);
    }

    // Draw button border with hover effect
    SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255);
    SDL_Rect border = {x, y, w, h};
    SDL_RenderDrawRect(renderer, &border);

    if (selected) {
        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 30);
        SDL_RenderFillRect(renderer, &border);
    }

    // Draw text with shadow
    SDL_Color shadow_color = {40, 40, 40, 180};
    render_text(renderer, font, text, x + w/2 + 1, y + (h-24)/2 + 1, shadow_color, 1);
    
    SDL_Color text_color = selected ? 
        (SDL_Color){0, 0, 180, 255} : 
        (SDL_Color){0, 0, 0, 255};
    
    render_text(renderer, font, text, x + w/2, y + (h-24)/2, text_color, 1);
}

void draw_background(SDL_Renderer *renderer) {
    if (!renderer) return;
    
    // Create a subtle gradient background
    for (int y = 0; y < 600; y++) {
        float factor = (float)y / 600.0f;
        Uint8 r = 255 - (Uint8)(25 * factor);
        Uint8 g = 255 - (Uint8)(15 * factor);
        Uint8 b = 255 - (Uint8)(5 * factor);
        
        SDL_SetRenderDrawColor(renderer, r, g, b, 255);
        SDL_RenderDrawLine(renderer, 0, y, 800, y);
    }
}

void draw_sticks(SDL_Renderer *renderer, int total_sticks, StickAnimation *animations) {
    if (!renderer || !animations) return;

    // Draw stick shadows with ambient occlusion
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    for (int i = 0; i < total_sticks; i++) {
        StickAnimation *anim = &animations[i];
        float shadow_intensity = 0.3f + (float)i / total_sticks * 0.7f;
        float shadow_offset = 5.0f + anim->scale * 2.0f;
        
        SDL_SetRenderDrawColor(renderer, 100, 50, 0, (Uint8)(128 * shadow_intensity));
        SDL_Rect shadow = {
            (int)(400 - (total_sticks * 30) / 2 + i * 30 + shadow_offset),
            200 + (int)shadow_offset,
            20,
            100
        };
        SDL_RenderFillRect(renderer, &shadow);
    }

    // Draw sticks with enhanced lighting
    for (int i = 0; i < total_sticks; i++) {
        StickAnimation *anim = &animations[i];
        int x = 400 - (total_sticks * 30) / 2 + i * 30;
        int y = 200;
        
        // Calculate shine effect
        float shine_pos = fmod(SDL_GetTicks() * 0.001f + i * 0.2f, 1.0f);
        
        // Draw stick with gradient and shine
        for (int j = 0; j < 100; j++) {
            float height_factor = j / 100.0f;
            float shine_factor = 1.0f - fabs(height_factor - shine_pos) * 5.0f;
            if (shine_factor < 0.0f) shine_factor = 0.0f;
            
            int r = 139 - (int)(j * 0.5f) + (int)(shine_factor * 60);
            int g = 69 - (int)(j * 0.3f) + (int)(shine_factor * 40);
            int b = 19 + (int)(shine_factor * 20);
            
            SDL_SetRenderDrawColor(renderer, r, g, b, 255);
            SDL_Rect segment = {
                x + (int)(sin(anim->rotation) * j * 0.1f),
                y + j,
                20,
                1
            };
            SDL_RenderFillRect(renderer, &segment);
        }
        
        // Draw stick head with highlight
        SDL_SetRenderDrawColor(renderer, 160, 82, 45, 255);
        SDL_Rect head = {x - 2, y - 5, 24, 10};
        SDL_RenderFillRect(renderer, &head);
        
        SDL_SetRenderDrawColor(renderer, 180, 102, 65, 255);
        SDL_Rect highlight = {x - 1, y - 4, 22, 4};
        SDL_RenderFillRect(renderer, &highlight);
    }
    
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);
}