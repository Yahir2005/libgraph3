#include "libgraph3.h"
#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <stdio.h>
#include <math.h> /* Nueva cabecera para trigonometría */

#define PI 3.14159265358979323846

static SDL_Window* window = NULL;
static SDL_Renderer* renderer = NULL;
static TTF_Font* default_font = NULL;

static int current_color = WHITE;
static int current_bkcolor = BLACK;

static int cp_x = 0; 
static int cp_y = 0; 

static int key_buffer = 0;

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

int kbhit(void) {
    /* Si ya hay una tecla esperando en el buzón, avisamos que sí hay un "hit" */
    if (key_buffer != 0) return 1;

    SDL_Event event;
    /* SDL_PollEvent revisa la cola de eventos al instante SIN pausar el programa */
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_EVENT_QUIT) {
            key_buffer = 27; /* Simulamos la tecla ESC */
            return 1;
        }
        if (event.type == SDL_EVENT_KEY_DOWN) {
            /* Guardamos la tecla en el buzón para que getch() la recoja después */
            key_buffer = (int)event.key.key;
            return 1;
        }
    }
    return 0;
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

    cp_x = 0;
    cp_y = 0;
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

/* --- MEDIDORES DE ENTORNO --- */
int getmaxx(void){
    if(!window) return 0;
    int w,h;
    SDL_GetWindowSize(window, &w, &h);
    return w - 1;
}

int getmaxy(void) {
    if (!window) return 0;
    int w, h;
    SDL_GetWindowSize(window, &w, &h);
    return h - 1; 
}

int textwidth(const char *text) {
    if (!default_font || !text) return 0;
    int w = 0, h = 0;
    TTF_GetStringSize(default_font, text, 0, &w, &h);
    return w;
}

int textheight(const char *text) {
    if (!default_font || !text) return 0;
    int w = 0, h = 0;
    TTF_GetStringSize(default_font, text, 0, &w, &h);
    return h;
}

/* --- CURSOR GRÁFICO (CP) --- */

void moveto(int x, int y) {
    /* Simplemente levantamos el lápiz y lo movemos a la nueva coordenada */
    cp_x = x;
    cp_y = y;
}

void lineto(int x, int y) {
    /* Dibujamos una línea desde donde está el lápiz hasta el nuevo punto */
    line(cp_x, cp_y, x, y);
    /* Actualizamos la posición del lápiz para que se quede en el final del trazo */
    cp_x = x;
    cp_y = y;
}

void moverel(int dx, int dy) {
    /* Movemos el lápiz relativamente sumando a su posición actual */
    cp_x += dx;
    cp_y += dy;
}

void linerel(int dx, int dy) {
    /* Dibujamos sumando a la posición actual */
    line(cp_x, cp_y, cp_x + dx, cp_y + dy);
    cp_x += dx;
    cp_y += dy;
}

/* --- GEOMETRÍA AVANZADA --- */

void drawpoly(int numpoints, int *polypoints) {
    if (numpoints < 2 || !polypoints) return;
    
    /* El arreglo polypoints tiene el formato [x1, y1, x2, y2, ...] */
    /* Iteramos conectando cada punto con el siguiente */
    for (int i = 0; i < numpoints - 1; i++) {
        line(polypoints[i * 2], polypoints[i * 2 + 1],
             polypoints[(i + 1) * 2], polypoints[(i + 1) * 2 + 1]);
    }
}

void ellipse(int x, int y, int stangle, int endangle, int xradius, int yradius) {
    /* Si el radio es 0, no hay nada que dibujar */
    if (xradius <= 0 || yradius <= 0) return;

    BGIColor fg = palette[current_color];
    SDL_SetRenderDrawColor(renderer, fg.r, fg.g, fg.b, SDL_ALPHA_OPAQUE);

    /* Calculamos la suavidad de la curva dependiendo de su tamaño */
    float step = 1.0f / (float)(xradius > yradius ? xradius : yradius);
    
    /* Convertimos los ángulos de grados BGI a radianes */
    float start_rad = stangle * PI / 180.0f;
    float end_rad = endangle * PI / 180.0f;

    /* Si el ángulo final es menor, asumimos que da la vuelta completa */
    if (end_rad < start_rad) end_rad += 2.0f * PI;

    /* Punto de inicio */
    float last_px = x + xradius * cosf(start_rad);
    float last_py = y - yradius * sinf(start_rad); 

    /* Trazamos pequeños segmentos de línea a lo largo de la curva polar */
    for (float theta = start_rad + step; theta <= end_rad; theta += step) {
        float px = x + xradius * cosf(theta);
        float py = y - yradius * sinf(theta);
        SDL_RenderLine(renderer, last_px, last_py, px, py);
        last_px = px;
        last_py = py;
    }
    
    /* Para evitar huecos al final de la curva por el redondeo del ciclo */
    float final_px = x + xradius * cosf(end_rad);
    float final_py = y - yradius * sinf(end_rad);
    SDL_RenderLine(renderer, last_px, last_py, final_px, final_py);
    
    SDL_RenderPresent(renderer);
}

void arc(int x, int y, int stangle, int endangle, int radius) {
    /* Un arco es simplemente una elipse donde ambos radios son iguales */
    ellipse(x, y, stangle, endangle, radius, radius);
}

/* --- LECTORES DE ESTADO (GETTERS) --- */

int getx(void) {
    return cp_x;
}

int gety(void) {
    return cp_y;
}

int getcolor(void) {
    return current_color;
}

int getbkcolor(void) {
    return current_bkcolor;
}