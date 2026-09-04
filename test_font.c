#include "SDL3/SDL.h"
#include "SDL3_ttf/SDL_ttf.h"
#include <stdio.h>
int main() {
    SDL_Init(SDL_INIT_VIDEO);
    TTF_Init();
    TTF_Font* font = TTF_OpenFont("/usr/local/share/libgraph3/font.ttf", 18);
    if (!font) {
        printf("Error loading font: %s\n", SDL_GetError());
    } else {
        printf("Font loaded successfully!\n");
    }
    return 0;
}
