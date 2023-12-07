#include <SDL2/SDL.h>

// #include "waveshare/Config/DEV_Config.h"
// #include "waveshare/LCD/LCD_1in47.h"
// #include "waveshare/GUI/GUI_Paint.h"

#include "DEV_Config.h"
#include "LCD_1in47.h"
#include "GUI_Paint.h"

#define PIXEL_FORMAT SDL_PIXELFORMAT_RGBA8888
#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 172

#ifdef USE_WAVESHARE

#ifndef RGB
#define  RGB(r,g,b)         (((r>>3)<<11)|((g>>2)<<5)|(b>>3))
#endif

UWORD *BlackImage;

void init()
{
    if (DEV_ModuleInit() != 0)
    {
        DEV_ModuleExit();
        exit(0);
    }

    /* LCD Init */
    printf("1.47inch LCD demo...\r\n");
    LCD_1IN47_Init(HORIZONTAL);
    LCD_1IN47_Clear(BLACK);
    LCD_SetBacklight(1023);

    UDOUBLE Imagesize = LCD_1IN47_HEIGHT * LCD_1IN47_WIDTH * 2;
    printf("Imagesize = %d\r\n", Imagesize);
    if ((BlackImage = (UWORD *)malloc(Imagesize)) == NULL)
    {
        printf("Failed to apply for black memory...\r\n");
        exit(0);
    }
    /*1.Create a new image cache named IMAGE_RGB and fill it with white*/
    Paint_NewImage(BlackImage, LCD_1IN47_WIDTH, LCD_1IN47_HEIGHT, 90, BLACK, 16);
    Paint_Clear(BLACK);
}

void quit()
{
    free(BlackImage);
    BlackImage = NULL;
    DEV_ModuleExit();
}

void render(SDL_Renderer *renderer, SDL_Texture *texture)
{
    printf("render waveshare\n");

    // Paint_DrawPoint(2, 18, BLACK, DOT_PIXEL_1X1, DOT_FILL_RIGHTUP);
    // Paint_DrawPoint(2, 20, BLACK, DOT_PIXEL_2X2, DOT_FILL_RIGHTUP);
    // Paint_DrawPoint(2, 23, BLACK, DOT_PIXEL_3X3, DOT_FILL_RIGHTUP);
    // Paint_DrawPoint(2, 28, BLACK, DOT_PIXEL_4X4, DOT_FILL_RIGHTUP);
    // Paint_DrawRectangle(20, 5, 80, 65, RED, DOT_PIXEL_2X2, DRAW_FILL_EMPTY);

    // BlackImage = texture->pixels;

    // SDL_Surface *surface;
    // Uint32 r_mask, g_mask, b_mask, a_mask;
    // int bbp;
    // SDL_PixelFormatEnumToMasks(PIXEL_FORMAT, &bbp, &r_mask, &g_mask, &b_mask, &a_mask);
    // surface = SDL_CreateRGBSurface(0, SCREEN_WIDTH, SCREEN_HEIGHT, bbp, r_mask, g_mask, b_mask, a_mask);
    // SDL_RenderReadPixels(renderer, NULL, PIXEL_FORMAT, (void *)surface->pixels, surface->pitch);

    // LCD_1IN47_Display(surface->pixels);





    SDL_Surface *surface;
    Uint32 r_mask, g_mask, b_mask, a_mask;
    int bbp;
    SDL_PixelFormatEnumToMasks(PIXEL_FORMAT, &bbp, &r_mask, &g_mask, &b_mask, &a_mask);
    surface = SDL_CreateRGBSurface(0, SCREEN_WIDTH, SCREEN_HEIGHT, bbp, r_mask, g_mask, b_mask, a_mask);
    SDL_RenderReadPixels(renderer, NULL, PIXEL_FORMAT, (void *)surface->pixels, surface->pitch);

    for (int x = 0; x < SCREEN_WIDTH; x++)
    {
        for (int y = 0; y < SCREEN_HEIGHT; y++)
        {
            // int *pixel = (int *)(surface->pixels + y * SCREEN_WIDTH + x);
            SDL_Color color;
            SDL_GetRGBA(((Uint32 *)surface->pixels)[x + y * SCREEN_WIDTH], surface->format, &color.r, &color.g, &color.b, &color.a);
            // printf("x %d y %d = r %d g %d b %d\n", x, y, color.r, color.g, color.b);
            Paint_SetPixel(x, y, RGB(color.r, color.g, color.b));
        }
    }
    LCD_1IN47_Display(BlackImage);




    // LCD_1IN47_Display(BlackImage);

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

#else
void init() {}
void quit() {}

void render(SDL_Renderer *renderer, SDL_Texture *texture)
{
    printf("render SDL only\n");

    // SDL_LockTexture(texture, NULL, (void **)&pixels, &pitch);

    SDL_Surface *surface;
    Uint32 r_mask, g_mask, b_mask, a_mask;
    int bbp;
    SDL_PixelFormatEnumToMasks(PIXEL_FORMAT, &bbp, &r_mask, &g_mask, &b_mask, &a_mask);
    surface = SDL_CreateRGBSurface(0, SCREEN_WIDTH, SCREEN_HEIGHT, bbp, r_mask, g_mask, b_mask, a_mask);
    SDL_RenderReadPixels(renderer, NULL, PIXEL_FORMAT, (void *)surface->pixels, surface->pitch);

    for (int x = 0; x < SCREEN_WIDTH; x++)
    {
        for (int y = 0; y < SCREEN_HEIGHT; y++)
        {
            // int *pixel = (int *)(surface->pixels + y * SCREEN_WIDTH + x);
            SDL_Color color;
            SDL_GetRGBA(((Uint32 *)surface->pixels)[x + y * SCREEN_WIDTH], surface->format, &color.r, &color.g, &color.b, &color.a);
            printf("x %d y %d = r %d g %d b %d\n", x, y, color.r, color.g, color.b);
        }
    }

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

#endif

int main(int argc, char *argv[])
{
    init();

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

    for (int i = 0; i < 5; i++)
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

    quit();
}