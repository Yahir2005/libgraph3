#include "libgraph3.h"
#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <stdio.h>

static SDL_Window* window = NULL;
static SDL_Renderer* renderer = NULL;
static TTF_Font* default_font = NULL;

static int current_color = WHITE;
static int current_bkcolor = BLACK;

typedef struct { Uint8 r, g, b; } BGIColor;
static BGIColor palette[16] = {
    {0, 0, 0},       {0, 0, 170},     {0, 170, 0},     {0, 170, 170},
    {170, 0, 0},     {170, 0, 170},   {170, 85, 0},    {170, 170, 170},
    {85, 85, 85},    {85, 85, 255},   {85, 255, 85},   {85, 255, 255},
    {255, 85, 85},   {255, 85, 255},  {255, 255, 85},  {255, 255, 255}
};

void initgraph(int *graphdriver, int *graphmode, const char *pathtodriver) {
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        printf("Error inicializando SDL: %s\n", SDL_GetError());
        return;
    }
    
    if (!SDL_CreateWindowAndRenderer("libgraph3 - Motor SDL3", 640, 480, 0, &window, &renderer)) {
        printf("Error creando ventana: %s\n", SDL_GetError());
        return;
    }

    /* Inicializamos el motor tipográfico */
    if (!TTF_Init()) {
        printf("Error inicializando SDL_ttf: %s\n", SDL_GetError());
        return;
    }

    /* Cargamos una fuente estándar de Ubuntu (DejaVu Sans) a tamaño 18 */
    default_font = TTF_OpenFont("/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf", 18);
    if (!default_font) {
        printf("Advertencia: No se pudo cargar la fuente por defecto.\n");
    }

    cleardevice();
}

void closegraph(void) {
    if (default_font) TTF_CloseFont(default_font);
    TTF_Quit(); /* Apagamos el motor de texto */
    if (renderer) SDL_DestroyRenderer(renderer);
    if (window) SDL_DestroyWindow(window);
    SDL_Quit();
}

void delay(int millis) {
    SDL_Delay(millis);
}

int getch(void) {
    SDL_Event event;
    while (SDL_WaitEvent(&event)) {
        if (event.type == SDL_EVENT_QUIT) return 27;
        if (event.type == SDL_EVENT_KEY_DOWN) return (int)event.key.key;
    }
    return 0;
}

void setcolor(int color) { if (color >= 0 && color <= 15) current_color = color; }
void setbkcolor(int color) { if (color >= 0 && color <= 15) current_bkcolor = color; }

void cleardevice(void) {
    BGIColor bg = palette[current_bkcolor];
    SDL_SetRenderDrawColor(renderer, bg.r, bg.g, bg.b, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(renderer);
    SDL_RenderPresent(renderer);
}

void line(int x1, int y1, int x2, int y2) {
    BGIColor fg = palette[current_color];
    SDL_SetRenderDrawColor(renderer, fg.r, fg.g, fg.b, SDL_ALPHA_OPAQUE);
    SDL_RenderLine(renderer, x1, y1, x2, y2);
    SDL_RenderPresent(renderer);
}

void putpixel(int x, int y, int color) {
    int old_color = current_color;
    setcolor(color);
    BGIColor fg = palette[current_color];
    SDL_SetRenderDrawColor(renderer, fg.r, fg.g, fg.b, SDL_ALPHA_OPAQUE);
    SDL_RenderPoint(renderer, (float)x, (float)y);
    SDL_RenderPresent(renderer);
    setcolor(old_color);
}

void rectangle(int left, int top, int right, int bottom) {
    BGIColor fg = palette[current_color];
    SDL_SetRenderDrawColor(renderer, fg.r, fg.g, fg.b, SDL_ALPHA_OPAQUE);
    SDL_FRect rect = { (float)left, (float)top, (float)(right - left), (float)(bottom - top) };
    SDL_RenderRect(renderer, &rect);
    SDL_RenderPresent(renderer);
}

void circle(int xc, int yc, int r) {
    BGIColor fg = palette[current_color];
    SDL_SetRenderDrawColor(renderer, fg.r, fg.g, fg.b, SDL_ALPHA_OPAQUE);
    int x = 0, y = r;
    int d = 3 - 2 * r;
    while (y >= x) {
        SDL_RenderPoint(renderer, (float)(xc + x), (float)(yc + y));
        SDL_RenderPoint(renderer, (float)(xc - x), (float)(yc + y));
        SDL_RenderPoint(renderer, (float)(xc + x), (float)(yc - y));
        SDL_RenderPoint(renderer, (float)(xc - x), (float)(yc - y));
        SDL_RenderPoint(renderer, (float)(xc + y), (float)(yc + x));
        SDL_RenderPoint(renderer, (float)(xc - y), (float)(yc + x));
        SDL_RenderPoint(renderer, (float)(xc + y), (float)(yc - x));
        SDL_RenderPoint(renderer, (float)(xc - y), (float)(yc - x));
        x++;
        if (d > 0) { y--; d = d + 4 * (x - y) + 10; } 
        else { d = d + 4 * x + 6; }
    }
    SDL_RenderPresent(renderer);
}

/* --- LA NUEVA FUNCIÓN DE TEXTO --- */
void outtextxy(int x, int y, const char *text) {
    if (!default_font || !text) return;

    /* Obtenemos el color actual de BGI y lo convertimos a SDL_Color */
    BGIColor fg = palette[current_color];
    SDL_Color textColor = { fg.r, fg.g, fg.b, 255 };

    /* Creamos una superficie (en RAM) con el texto renderizado y suavizado */
    SDL_Surface* textSurface = TTF_RenderText_Blended(default_font, text, 0, textColor);
    if (!textSurface) return;

    /* Subimos esa superficie a la memoria de video (VRAM) como una textura */
    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    
    /* Preparamos el rectángulo de destino usando el tamaño real de la fuente */
    SDL_FRect renderQuad = { (float)x, (float)y, (float)textSurface->w, (float)textSurface->h };

    /* Dibujamos la textura en la pantalla */
    SDL_RenderTexture(renderer, textTexture, NULL, &renderQuad);
    SDL_RenderPresent(renderer);

    /* Limpiamos la memoria temporal */
    SDL_DestroyTexture(textTexture);
    SDL_DestroySurface(textSurface);
}