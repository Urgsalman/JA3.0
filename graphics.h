#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "game.h"

// Core drawing functions
void draw_background(SDL_Renderer *renderer);
void draw_sticks(SDL_Renderer *renderer, int total_sticks, StickAnimation *animations);
void draw_button(SDL_Renderer *renderer, TTF_Font *font, const char *text, 
                int x, int y, int w, int h, int selected);

// Animation functions
void init_stick_animations(StickAnimation *animations, int count);
void update_stick_animations(StickAnimation *animations, int count);

// Helper drawing functions
void render_text(SDL_Renderer *renderer, TTF_Font *font, const char *text, 
                int x, int y, SDL_Color color, int centered);
void draw_particle_effects(SDL_Renderer *renderer, int x, int y);
void draw_winning_line(SDL_Renderer *renderer, int x1, int y1, int x2, int y2); // Updated declaration

// UI Effects
void draw_shadow(SDL_Renderer *renderer, int x, int y, int w, int h, int blur_radius);
void draw_gradient_rect(SDL_Renderer *renderer, int x, int y, int w, int h, 
                       SDL_Color start_color, SDL_Color end_color);
void draw_hover_effect(SDL_Renderer *renderer, int x, int y, int w, int h, float intensity);

#endif // GRAPHICS_H
