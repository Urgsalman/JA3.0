CC = gcc
CFLAGS = -Wall -Wextra -std=c11 `sdl2-config --cflags` -Iinclude
LDFLAGS = `sdl2-config --libs` -lSDL2_ttf
SRC = SDL_main.c credits.c rules.c game.c graphics.c menu.c scores.c ai.c game_over.c
OBJ = $(SRC:.c=.o)
TARGET = matchstick_game

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(OBJ) -o $(TARGET) $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(TARGET)

run: all
	./$(TARGET)
