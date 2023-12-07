#include <SDL2/SDL.h>

#define PIXEL_FORMAT SDL_PIXELFORMAT_RGBA8888
#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 172

void render(SDL_Renderer *renderer, SDL_Texture *texture)
{
    // During the whole rendering process, we render into a texture
    // Only at the end, we push the texture to the screen
    //
    // Set renderer pointing to screen
    SDL_SetRenderTarget(renderer, NULL);
    // Copy texture to renderer pointing on the screen
    SDL_RenderCopy(renderer, texture, NULL, NULL);
    // Present renderer
    SDL_RenderPresent(renderer);
    // Set renderer pointinng to texture
    SDL_SetRenderTarget(renderer, texture);
}

int main(int argc, char *argv[])
{

    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Could not initialize SDL: %s", SDL_GetError());
        return 1;
    }

    SDL_Window *window = SDL_CreateWindow(
        "Demo",
        SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
        SCREEN_WIDTH, SCREEN_HEIGHT,
        SDL_WINDOW_SHOWN
#ifdef IS_RPI
            | SDL_WINDOW_FULLSCREEN
#endif
    );

    if (window == NULL)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Could not create window: %s", SDL_GetError());
        return -1;
    }

    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

    // SDL_Texture *texture = (SDL_Texture *)setTextureRenderer(styles.screen);
    SDL_Texture *texture = SDL_CreateTexture(renderer, PIXEL_FORMAT, SDL_TEXTUREACCESS_TARGET, SCREEN_WIDTH, SCREEN_HEIGHT);
    SDL_SetRenderTarget(renderer, texture);

    for (int i = 0; i < 10; i++)
    {
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        SDL_Rect rect = {i * 10, 0, 50, 50};
        SDL_RenderFillRect(renderer, &rect);

        render(renderer, texture);
        SDL_Delay(500);
    }

    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
}