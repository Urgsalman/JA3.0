#include "menu.h"
#include "graphics.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <math.h>

static MenuAnimation button_animations[5];
static float menu_time = 0;

void init_menu_animations() {
    for (int i = 0; i < 5; i++) {
        button_animations[i].scale = 1.0f;
        button_animations[i].rotation = 0.0f;
        button_animations[i].hover_effect = 0.0f;
    }
}

void update_menu_animations(int selected_option) {
    menu_time += 0.016f; // Assuming 60 FPS

    for (int i = 0; i < 5; i++) {
        if (i == selected_option) {
            button_animations[i].hover_effect += (1.0f - button_animations[i].hover_effect) * 0.1f;
        } else {
            button_animations[i].hover_effect *= 0.9f;
        }

        button_animations[i].scale = 1.0f + sin(menu_time * 2 + i * 0.5f) * 0.02f;
        button_animations[i].rotation = sin(menu_time + i * 0.7f) * 2.0f;
    }
}

void draw_title(SDL_Renderer *renderer, TTF_Font *font) {
    SDL_Color rainbow = {255, 0, 0, 255}; // Simple red color for title
    const char *title = "Matches Game";

    SDL_Surface *surface = TTF_RenderText_Blended(font, title, rainbow);
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);

    int title_y = 50 + sin(menu_time * 3) * 10;
    SDL_Rect title_rect = {
        400 - surface->w / 2,
        title_y,
        surface->w,
        surface->h
    };

    SDL_RenderCopyEx(renderer, texture, NULL, &title_rect, sin(menu_time) * 5, NULL, SDL_FLIP_NONE);

    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);
}

void show_menu(SDL_Renderer *renderer, TTF_Font *font, int selected_option) {
    static int initialized = 0;
    if (!initialized) {
        init_menu_animations();
        initialized = 1;
    }

    update_menu_animations(selected_option);

    draw_background(renderer);
    draw_title(renderer, font);

    const char *options[] = {
        "Player vs Player",
        "Player vs Computer",
        "Rules",
        "Credits",
        "Quit"
    };

    for (int i = 0; i < 5; i++) {
        MenuAnimation *anim = &button_animations[i];
        int y_base = 200 + i * 80;
        int y_offset = sin(menu_time * 2 + i * 0.5f) * 5;

        int button_width = 300 + anim->hover_effect * 20;
        int button_height = 60 + anim->hover_effect * 10;

        int x = 400 - button_width / 2;
        int y = y_base + y_offset;

        SDL_Color button_color = {0, 0, 0, 255};
        draw_button(renderer, font, options[i], x, y, button_width * anim->scale, button_height * anim->scale, i == selected_option);

        if (i == selected_option) {
            draw_particle_effects(renderer, x + button_width / 2, y + button_height / 2);
        }
    }
}

int handle_menu_events(SDL_Event *event, int *selected_option) {
    if (event->type == SDL_MOUSEMOTION) {
        int x = event->motion.x, y = event->motion.y;
        for (int i = 0; i < 5; i++) {
            int y_base = 200 + i * 80;
            if (x >= 250 && x <= 550 && y >= y_base && y <= y_base + 60) {
                *selected_option = i;
                break;
            }
        }
    }

    if (event->type == SDL_MOUSEBUTTONDOWN) {
        int x = event->button.x, y = event->button.y;
        for (int i = 0; i < 5; i++) {
            int y_base = 200 + i * 80;
            if (x >= 250 && x <= 550 && y >= y_base && y <= y_base + 60) {
                return 1;
            }
        }
    }

    return 0;
}