#include <stdio.h>
#include "types.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_image.h>

#define WINDOW_WIDTH (740)
#define WINDOW_HEIGHT (1024)

// speed in pixels/second
#define SPEED (50)
#define GRAVITY_SPEED (300)

#define DEBUG 1
#define DEBUG_FONT_SIZE 16


struct DebugInfo
{
    SDL_Texture *texture;
    SDL_Rect *location;
};

struct Game 
{
    bool isRunning;
    SDL_Window *window;
    SDL_Renderer *render;
    TTF_Font *font;
    SDL_Texture *falcon9Texture;
    DebugInfo *debug;
};

struct KeyboardState
{
    int up;
    int down;
    int right;
    int left;
};

void initGame(Game *game);
void destroyGame(Game *game);
void processEvents(Game *game, KeyboardState *keyboardState);
SDL_Texture* getDebugTexture(char *text, Game *game);

SDL_Texture* loadTexture(const char *filename, SDL_Renderer *render);