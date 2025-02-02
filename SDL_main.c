#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>
#include "menu.h"
#include "game.h"
#include "rules.h"
#include "credits.h"
#include "scores.h"
#include "game_over.h"
#include "difficulty_menu.h"

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

int main(int argc, char *argv[]) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL initialization error: %s\n", SDL_GetError());
        return 1;
    }

    if (TTF_Init() < 0) {
        printf("SDL_ttf initialization error: %s\n", TTF_GetError());
        SDL_Quit();
        return 1;
    }

    SDL_Window *window = SDL_CreateWindow(
        "Matches Game",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        WINDOW_WIDTH,
        WINDOW_HEIGHT,
        SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL
    );

    if (!window) {
        printf("Window creation error: %s\n", SDL_GetError());
        TTF_Quit();
        SDL_Quit();
        return 1;
    }

    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!renderer) {
        printf("Renderer creation error: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        TTF_Quit();
        SDL_Quit();
        return 1;
    }

    // Enable alpha blending for better graphics
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

    TTF_Font *font = TTF_OpenFont("resources/fonts/arial.ttf", 24);
    if (!font) {
        printf("Font loading error: %s\n", TTF_GetError());
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        TTF_Quit();
        SDL_Quit();
        return 1;
    }

    int quit = 0;
    int selected_option = 0;
    int game_state = 0; // 0: menu, 1: game, 2: rules, 3: credits, 4: difficulty menu, 5: game over
    
    int total_sticks = 21;
    int player_turn = 0;
    int game_mode = 0;
    int ai_difficulty = 1;
    Scores scores = charger_scores();

    Uint32 frame_start;
    int frame_time;

    while (!quit) {
        frame_start = SDL_GetTicks();
        
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                quit = 1;
            }

            switch (game_state) {
                case 0: // Main Menu
                    if (handle_menu_events(&event, &selected_option)) {
                        switch (selected_option) {
                            case 0: // Player vs Player
                                game_state = 1;
                                game_mode = 0;
                                break;
                            case 1: // Player vs Computer
                                game_state = 4; // Go to difficulty menu
                                game_mode = 1;
                                break;
                            case 2: // Rules
                                game_state = 2;
                                break;
                            case 3: // Credits
                                game_state = 3;
                                break;
                            case 4: // Quit
                                quit = 1;
                                break;
                        }
                    }
                    break;

                case 1: // Game
                    if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE) {
                        game_state = 0;
                        total_sticks = 21;
                        player_turn = 0;
                    }
                    break;

                case 2: // Rules
                    if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE) {
                        game_state = 0;
                    }
                    break;

                case 3: // Credits
                    if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE) {
                        game_state = 0;
                    }
                    break;

                case 4: // Difficulty Menu
                    if (handle_difficulty_menu_events(&event, &selected_option)) {
                        if (selected_option >= 0 && selected_option <= 2) {
                            ai_difficulty = selected_option;
                            game_state = 1;
                        } else if (selected_option == -1) {
                            game_state = 0;
                        }
                    }
                    break;

                case 5: // Game Over
                    if (handle_game_over_events(&event, &selected_option)) {
                        if (selected_option == 0) {
                            game_state = 1;
                            total_sticks = 21;
                            player_turn = 0;
                        } else {
                            game_state = 0;
                        }
                    }
                    break;
            }
        }

        // Clear screen with a nice gradient background
        SDL_SetRenderDrawColor(renderer, 230, 240, 255, 255);
        SDL_RenderClear(renderer);

        // Render current state
        switch (game_state) {
            case 0:
                show_menu(renderer, font, selected_option);
                break;
            case 1:
                play_game(renderer, font, &total_sticks, &player_turn, &scores, game_mode, ai_difficulty);
                if (check_winner(total_sticks)) {
                    game_state = 5;
                }
                break;
            case 2:
                show_rules(renderer, font);
                break;
            case 3:
                show_credits(renderer, font);
                break;
            case 4:
                show_difficulty_menu(renderer, font, selected_option);
                break;
            case 5:
                show_game_over(renderer, font, scores, game_mode, player_turn);
                break;
        }

        SDL_RenderPresent(renderer);

        // Cap framerate to 60 FPS
        frame_time = SDL_GetTicks() - frame_start;
        if (frame_time < 16) {
            SDL_Delay(16 - frame_time);
        }
    }

    TTF_CloseFont(font);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    SDL_Quit();

    return 0;
}