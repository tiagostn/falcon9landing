#include "falcon9landing.h"

void initGame(Game *game)
{
    game->isRunning = true;

    // init SDL
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        printf("error initializing SDL: %s\n", SDL_GetError());
        game->isRunning = false;
    }
    // init SDL_TTF
    if (TTF_Init() == -1)
    {
        printf("error initializing SDL_TTF: %s\n", TTF_GetError());
        game->isRunning = false;
    }
    // create window handler
    game->window = SDL_CreateWindow("SDL Learn", 
                                          SDL_WINDOWPOS_CENTERED, 
                                          SDL_WINDOWPOS_CENTERED, 
                                          WINDOW_WIDTH, 
                                          WINDOW_HEIGHT, 0);
    if (!game->window)
    {
        printf("error creating window: %s\n", SDL_GetError());
        game->isRunning = false;
    }

    // create render
    uint32 renderFlags = SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC;
    game->render = SDL_CreateRenderer(game->window, -1, renderFlags);
    if (!game->render)
    {
        printf("error creating render: %s\n", SDL_GetError());
        game->isRunning = false;
    }

    // Load Section
    // load image
    game->falcon9Texture = loadTexture("resources/falcon9-stage1-small.png", game->render);

    // load font
    char const *fontname = "resources/DejaVuSansMono.ttf";
    // char const *fontname = "resources/Arial.ttf";
    game->font = TTF_OpenFont(fontname, DEBUG_FONT_SIZE);
    if (!game->font)
    {
        printf("error loading font %s: %s\n", fontname, TTF_GetError());
        game->isRunning = false;
    }

    game->debug = (DebugInfo*)malloc(sizeof(DebugInfo));
    
}

void destroyGame(Game *game)
{
    // clean up resources
    if (game->font) TTF_CloseFont(game->font);
    if (game->debug){
        if (game->debug->texture) SDL_DestroyTexture(game->debug->texture);
        free(game->debug);
    }
    if (game->falcon9Texture) SDL_DestroyTexture(game->falcon9Texture);
    if (game->render) SDL_DestroyRenderer(game->render);
    if (game->window) SDL_DestroyWindow(game->window);
    TTF_Quit();
    SDL_Quit();
}

SDL_Texture* getDebugTexture(char *text, Game *game)
{
    // Text Surface
    SDL_Color fgcolor={0x00ff, 0xff, 0xff}, bgcolor={0, 0, 0};
    SDL_Surface* debud_surface = TTF_RenderText_Blended(game->font, text, fgcolor);
    if (!debud_surface)
    {
        printf("error creating debug surface: %s\n", TTF_GetError());
        game->isRunning = false;
    }
    SDL_Rect debug_location = {0, 0, 100, DEBUG_FONT_SIZE};
    // Debug Texture
    game->debug->texture = SDL_CreateTextureFromSurface(game->render, debud_surface);
    SDL_FreeSurface(debud_surface);
    if (!game->debug->texture)
    {
        printf("error creating debug texture: %s\n", TTF_GetError());
        game->isRunning = false;
    }
    SDL_Rect dest = {};
    SDL_QueryTexture(game->debug->texture, 0, 0, &dest.w, &dest.h);

    game->debug->location = &dest;
}

void processKeyboardEvent(SDL_KeyboardEvent event, SDL_EventType eventType, KeyboardState* state)
{
    int active = eventType == SDL_KEYDOWN;    
    switch (event.keysym.scancode)
    {
        case SDL_SCANCODE_UP:
        case SDL_SCANCODE_W:
            state->up = active;
            break;
        case SDL_SCANCODE_DOWN:
        case SDL_SCANCODE_S:
            state->down = active;
            break;
        case SDL_SCANCODE_LEFT:
        case SDL_SCANCODE_A:
            state->left = active;
            break;
        case SDL_SCANCODE_RIGHT:
        case SDL_SCANCODE_D:
            state->right = active;
            break;
        default:
            break;
    }
    if (DEBUG)
        printf("[debug] - up: %d, down: %d, right: %d, left: %d\n", state->up, state->down, state->right, state->left);
}

void processEvents(Game *game, KeyboardState *keyboardState)
{
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {            
        switch (event.type)
        {
            case SDL_QUIT:
                game->isRunning = false;
                break;
            case SDL_KEYDOWN:
            {
                processKeyboardEvent(event.key, SDL_KEYDOWN, keyboardState);
            } break;
            case SDL_KEYUP:
            {
                processKeyboardEvent(event.key, SDL_KEYUP, keyboardState);                    
            } break;
            default:
                break;
        }
    }
}

SDL_Texture* loadTexture(const char *filename, SDL_Renderer *render)
{
    SDL_Surface *surface = IMG_Load(filename);
    if (!surface)
    {
        printf("error loading image: %s\n", SDL_GetError());
        return 0;
    }

    // create texture
    SDL_Texture *texture = SDL_CreateTextureFromSurface(render, surface);
    SDL_FreeSurface(surface);
    if (!texture)
    {
        printf("error creating texture: %s\n", SDL_GetError());
        return 0;
    }
    return texture;
}