#include "falcon9landing.h"

void info(const char* texto)
{
    if (DEBUG)
        printf("INFO  - %s", texto);
}

struct FirstStage
{
    struct 
    {
        int x;
        int y;
    } position;
    struct 
    {
        int x;
        int y;
    } velocity;
    struct 
    {
        int x;
        int y;
    } accelaration;
};

int main()
{

    Game game = {};
    initGame(&game);

    // struct to hold the position and size of the sprite
    SDL_Rect dest;

    // get and scale the dimensions of the texture
    SDL_QueryTexture(game.falcon9Texture, 0, 0, &dest.w, &dest.h);

    // position sprite in the center of the window
    dest.x = (WINDOW_WIDTH - dest.w) / 2;

    FirstStage fs = {};
    fs.position.x = (WINDOW_WIDTH - dest.w) / 2;
    fs.position.y = (WINDOW_HEIGHT - dest.h) / 2;

    // keep track of which inputs are given
    KeyboardState keyboardState = {};

    // animation loop
    while (game.isRunning)
    {
        // process events
        processEvents(&game, &keyboardState);        

        // accelaration
        // if (fs.accelaration.y  >= 0 && fs.accelaration.y <= 10) fs.accelaration.y += 5;
        // if (fs.velocity.y >= 0) fs.velocity.y += fs.accelaration.y;

        // determine velocity
        //x_vel = y_vel = 0;
        if (keyboardState.up && !keyboardState.down) 
        {
            fs.velocity.y -= SPEED;
            // fs.accelaration.y -= 5;
        }
        if (keyboardState.down && !keyboardState.up) 
        {
            fs.velocity.y += SPEED;
            // fs.accelaration.y += 5;
        }
        if (keyboardState.left && !keyboardState.right) fs.velocity.x -= SPEED;
        if (keyboardState.right && !keyboardState.left) fs.velocity.x += SPEED;

        // update positions
        fs.position.x += fs.velocity.x / 60;
        //fs.position.y += (fs.velocity.y + GRAVITY_SPEED) / 60;
        fs.position.y += (fs.velocity.y) / 60;

        // collision detection with bounds
        if (fs.position.x <= 0) {fs.position.x=0; }
        if (fs.position.y <= 0) {fs.position.y=0; }
        if (fs.position.x >= WINDOW_WIDTH - dest.w) {fs.position.x = WINDOW_WIDTH - dest.w; }
        if (fs.position.y >= WINDOW_HEIGHT - dest.h) {fs.position.y = WINDOW_HEIGHT - dest.h; }

        // set the positions in the struct
        dest.y = (int) fs.position.y;
        dest.x = (int) fs.position.x;

        // Debug Texture
        char debugText[70];
        sprintf(debugText, 
                "pos.x=[%3d] | pos.y=[%3d] | vel.x=[%3d] | vel.y=[%3d]", 
                fs.position.x, fs.position.y, fs.velocity.x, fs.velocity.y);
        getDebugTexture(debugText, &game);

        // clear the window
        SDL_RenderClear(game.render);

        // draw debug box
        SDL_RenderCopy(game.render, game.debug->texture, 0, game.debug->location);

        // draw the image to the window
        SDL_RenderCopy(game.render, game.falcon9Texture, NULL, &dest);
        SDL_RenderPresent(game.render);

        // wait 1/60th of a second
        SDL_Delay(1000/60);
    }

    // clean up resources
    destroyGame(&game);
    return 0;
}
