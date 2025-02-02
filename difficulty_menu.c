#include "difficulty_menu.h"
#include "graphics.h"

void show_difficulty_menu(SDL_Renderer *renderer, TTF_Font *font, int selected_option) {
    // Set background color to a light blue
    SDL_SetRenderDrawColor(renderer, 230, 240, 255, 255);
    SDL_RenderClear(renderer);

    // Draw title
    SDL_Color title_color = {0, 0, 139, 255}; // Dark blue
    const char *title = "Select Difficulty";
    SDL_Surface *title_surface = TTF_RenderText_Blended(font, title, title_color);
    SDL_Texture *title_texture = SDL_CreateTextureFromSurface(renderer, title_surface);

    SDL_Rect title_rect = {
        400 - title_surface->w / 2,
        100,
        title_surface->w,
        title_surface->h
    };
    SDL_RenderCopy(renderer, title_texture, NULL, &title_rect);

    // Define difficulty options with descriptions
    const char *options[] = {"Easy", "Medium", "Hard"};
    const char *descriptions[] = {
        "Computer makes random moves",
        "Computer plays with basic strategy",
        "Computer plays perfectly"
    };

    // Draw each option with its description
    for (int i = 0; i < 3; i++) {
        SDL_Color button_color = (i == selected_option) ? 
            (SDL_Color){0, 100, 200, 255} : (SDL_Color){0, 0, 139, 255};

        // Draw the button with animation if selected
        int y_offset = (i == selected_option) ? -5 : 0;
        draw_button(renderer, font, options[i], 250, 200 + i * 100 + y_offset, 300, 60, i == selected_option);

        // Draw description
        SDL_Color desc_color = {100, 100, 100, 255};
        SDL_Surface *desc_surface = TTF_RenderText_Blended(font, descriptions[i], desc_color);
        SDL_Texture *desc_texture = SDL_CreateTextureFromSurface(renderer, desc_surface);

        SDL_Rect desc_rect = {
            400 - desc_surface->w / 2,
            270 + i * 100 + y_offset,
            desc_surface->w,
            desc_surface->h
        };
        SDL_RenderCopy(renderer, desc_texture, NULL, &desc_rect);

        SDL_FreeSurface(desc_surface);
        SDL_DestroyTexture(desc_texture);
    }

    // Draw "Back" button
    draw_button(renderer, font, "Back", 300, 500, 200, 50, 0);

    SDL_FreeSurface(title_surface);
    SDL_DestroyTexture(title_texture);
}

int handle_difficulty_menu_events(SDL_Event *event, int *selected_option) {
    if (event->type == SDL_MOUSEMOTION) {
        int x = event->motion.x;
        int y = event->motion.y;
        
        // Check difficulty buttons
        for (int i = 0; i < 3; i++) {
            if (x >= 250 && x <= 550 && y >= 200 + i * 100 && y <= 260 + i * 100) {
                *selected_option = i;
                return 0;
            }
        }
    }
    
    if (event->type == SDL_MOUSEBUTTONDOWN) {
        int x = event->button.x;
        int y = event->button.y;
        
        // Check difficulty buttons
        for (int i = 0; i < 3; i++) {
            if (x >= 250 && x <= 550 && y >= 200 + i * 100 && y <= 260 + i * 100) {
                return 1; // Return 1 to indicate selection made
            }
        }
        
        // Check "Back" button
        if (x >= 300 && x <= 500 && y >= 500 && y <= 550) {
            *selected_option = -1; // Special value for back
            return 1;
        }
    }
    
    return 0;
}