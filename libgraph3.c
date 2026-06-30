#include "libgraph3.h"
#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <stdio.h>
#include <math.h> /* Nueva cabecera para trigonometría */
#include <stdlib.h> /* Necesario para la memoria dinámica del floodfill */

#define PI 3.14159265358979323846

static SDL_Window* window = NULL;
static SDL_Renderer* renderer = NULL;
static TTF_Font* default_font = NULL;

static int current_color = WHITE;
static int current_bkcolor = BLACK;

static int cp_x = 0; 
static int cp_y = 0; 

static int key_buffer = 0;

static int current_fill_color = WHITE; /* Color del relleno */
static int current_fill_pattern = SOLID_FILL; /* Estilo del relleno */

/* Nuevas variables para el estado del texto */
static int current_text_horiz = LEFT_TEXT;
static int current_text_vert = TOP_TEXT;
static int current_text_size = 18; /* Tamaño por defecto en puntos */

/* Estado del ratón */
static int mouse_x = 0;
static int mouse_y = 0;
static int mouse_clicked = 0;

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

/* --- INTERACCIÓN Y EVENTOS --- */

static void procesar_eventos(void) {
    SDL_Event event;
    /* Extraemos todos los eventos pendientes a máxima velocidad */
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_EVENT_QUIT) {
            key_buffer = 27; /* ESC */
        } else if (event.type == SDL_EVENT_KEY_DOWN) {
            key_buffer = (int)event.key.key;
        } else if (event.type == SDL_EVENT_MOUSE_BUTTON_DOWN) {
            /* Guardamos la coordenada exacta del clic */
            mouse_x = (int)event.button.x;
            mouse_y = (int)event.button.y;
            mouse_clicked = 1; 
        }
    }
}

int kbhit(void) {
    procesar_eventos();
    return (key_buffer != 0) ? 1 : 0;
}

int getch(void) {
    while (key_buffer == 0) {
        procesar_eventos();
        SDL_Delay(10); /* Evita consumir el 100% del procesador */
    }
    int key = key_buffer;
    key_buffer = 0; 
    return key;
}

int ismouseclick(void) {
    procesar_eventos();
    int click = mouse_clicked;
    mouse_clicked = 0; /* "Consumimos" el clic para que no se dispare dos veces seguidas */
    return click;
}

int mousex(void) { return mouse_x; }
int mousey(void) { return mouse_y; }

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

    /* 1. Medimos cuántos píxeles ocupará este texto en pantalla con el tamaño actual */
    int text_w = 0, text_h = 0;
    TTF_GetStringSize(default_font, text, 0, &text_w, &text_h);

    /* 2. Aplicamos la justificación horizontal */
    int adj_x = x;
    if (current_text_horiz == CENTER_TEXT) {
        adj_x = x - (text_w / 2);
    } else if (current_text_horiz == RIGHT_TEXT) {
        adj_x = x - text_w;
    }

    /* 3. Aplicamos la justificación vertical */
    int adj_y = y;
    if (current_text_vert == CENTER_TEXT) {
        adj_y = y - (text_h / 2);
    } else if (current_text_vert == BOTTOM_TEXT) {
        adj_y = y - text_h;
    }

    /* 4. Renderizado estándar de SDL3_ttf con las coordenadas ajustadas */
    BGIColor fg = palette[current_color];
    SDL_Color textColor = { fg.r, fg.g, fg.b, 255 };

    SDL_Surface* textSurface = TTF_RenderText_Blended(default_font, text, 0, textColor);
    if (!textSurface) return;

    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    SDL_FRect renderQuad = { (float)adj_x, (float)adj_y, (float)textSurface->w, (float)textSurface->h };

    SDL_RenderTexture(renderer, textTexture, NULL, &renderQuad);
    SDL_RenderPresent(renderer);

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

/* --- FIGURAS CON RELLENO SÓLIDO --- */

void setfillstyle(int pattern, int color) {
    current_fill_pattern = pattern;
    if (color >= 0 && color <= 15) {
        current_fill_color = color;
    }
}

void bar(int left, int top, int right, int bottom) {
    /* Si el estilo es vacío, no pintamos el interior */
    if (current_fill_pattern == EMPTY_FILL) return;

    /* Obtenemos el color de relleno y lo preparamos para la tarjeta gráfica */
    BGIColor fill_bg = palette[current_fill_color];
    SDL_SetRenderDrawColor(renderer, fill_bg.r, fill_bg.g, fill_bg.b, SDL_ALPHA_OPAQUE);

    /* Calculamos las dimensiones del bloque */
    SDL_FRect rect = { (float)left, (float)top, (float)(right - left), (float)(bottom - top) };
    
    /* SDL_RenderFillRect dibuja el rectángulo completamente coloreado por dentro */
    SDL_RenderFillRect(renderer, &rect);
    SDL_RenderPresent(renderer);
}

void floodfill(int x, int y, int border) {
    /* 1. Descargamos la pantalla actual de la GPU a la RAM */
    SDL_Surface* raw_surf = SDL_RenderReadPixels(renderer, NULL);
    if (!raw_surf) return;

    /* 2. Estandarizamos el formato a ARGB8888 para manipular los bits directamente */
    SDL_Surface* surf = SDL_ConvertSurface(raw_surf, SDL_PIXELFORMAT_ARGB8888);
    SDL_DestroySurface(raw_surf);
    if (!surf) return;

    int w = surf->w;
    int h = surf->h;

    /* Validamos que el clic inicial esté dentro de la pantalla */
    if (x < 0 || x >= w || y < 0 || y >= h) {
        SDL_DestroySurface(surf);
        return;
    }

    /* 3. Preparamos los colores exactos a nivel de bit */
    BGIColor fill_bgi = palette[current_fill_color];
    Uint32 fill_color = (255 << 24) | (fill_bgi.r << 16) | (fill_bgi.g << 8) | fill_bgi.b;

    BGIColor border_bgi = palette[border];
    Uint32 target_border = (255 << 24) | (border_bgi.r << 16) | (border_bgi.g << 8) | border_bgi.b;

    Uint32* pixels = (Uint32*)surf->pixels;
    Uint32 start_color = pixels[y * w + x];

    /* Si hacemos clic en un borde, o en un lugar que ya está pintado, abortamos */
    if (start_color == target_border || start_color == fill_color) {
        SDL_DestroySurface(surf);
        return;
    }

    /* 4. Algoritmo DFS usando Pila Dinámica para no explotar la memoria de C */
    int max_stack = w * h * 4; 
    int* stack_x = (int*)malloc(max_stack * sizeof(int));
    int* stack_y = (int*)malloc(max_stack * sizeof(int));
    int stack_ptr = 0;

    /* Metemos el punto inicial a la pila */
    stack_x[stack_ptr] = x;
    stack_y[stack_ptr] = y;
    stack_ptr++;

    while (stack_ptr > 0) {
        stack_ptr--;
        int cx = stack_x[stack_ptr];
        int cy = stack_y[stack_ptr];

        if (cx < 0 || cx >= w || cy < 0 || cy >= h) continue;

        int idx = cy * w + cx;
        Uint32 current_pixel = pixels[idx];

        /* Si el píxel no es borde y no ha sido pintado, lo pintamos y revisamos sus vecinos */
        if (current_pixel != target_border && current_pixel != fill_color) {
            pixels[idx] = fill_color;
            
            /* Derecha, Izquierda, Abajo, Arriba */
            stack_x[stack_ptr] = cx + 1; stack_y[stack_ptr] = cy; stack_ptr++;
            stack_x[stack_ptr] = cx - 1; stack_y[stack_ptr] = cy; stack_ptr++;
            stack_x[stack_ptr] = cx; stack_y[stack_ptr] = cy + 1; stack_ptr++;
            stack_x[stack_ptr] = cx; stack_y[stack_ptr] = cy - 1; stack_ptr++;
        }
    }

    free(stack_x);
    free(stack_y);

    /* 5. Subimos la textura terminada de regreso a la Tarjeta Gráfica */
    SDL_Texture* tex = SDL_CreateTextureFromSurface(renderer, surf);
    SDL_RenderTexture(renderer, tex, NULL, NULL);
    SDL_RenderPresent(renderer);

    SDL_DestroyTexture(tex);
    SDL_DestroySurface(surf);
}

/* --- TEXTO DINÁMICO --- */

void settextstyle(int font, int direction, int charsize) {
    /* Por ahora ignoramos 'font' y 'direction' para mantener el estándar de la fuente DejaVu */
    /* En BGI, 'charsize' solía ser un multiplicador (1, 2, 3). Lo mapearemos a puntos reales */
    if (charsize <= 0) charsize = 1;
    current_text_size = charsize * 12; // Multiplicador base de 12 puntos por unidad

    if (default_font) {
        /* SDL3_ttf nos permite cambiar el tamaño de la fuente cargada en vivo */
        TTF_SetFontSize(default_font, (float)current_text_size);
    }
}

void settextjustify(int horiz, int vert) {
    if (horiz >= 0 && horiz <= 2) current_text_horiz = horiz;
    if (vert >= 0 && vert <= 2) current_text_vert = vert;
}

/* --- MEMORIA VISUAL Y SPRITES --- */

unsigned int imagesize(int left, int top, int right, int bottom) {
    int w = right - left;
    int h = bottom - top;
    if (w <= 0 || h <= 0) return 0;
    
    /* Guardamos 4 bytes para ancho, 4 para alto, y 4 bytes por cada píxel */
    return 8 + (w * h * 4);
}

void getimage(int left, int top, int right, int bottom, void *bitmap) {
    if (!bitmap) return;
    
    int w = right - left;
    int h = bottom - top;
    if (w <= 0 || h <= 0) return;

    /* 1. Guardamos las dimensiones en el "encabezado" (primeros 8 bytes del buffer) */
    int *header = (int *)bitmap;
    header[0] = w;
    header[1] = h;

    /* 2. El área de los píxeles empieza justo después del encabezado */
    Uint32 *pixel_data = (Uint32 *)((char *)bitmap + 8);

    /* 3. Le tomamos la "foto" a la tarjeta gráfica */
    SDL_Rect rect = { left, top, w, h };
    SDL_Surface* raw_surf = SDL_RenderReadPixels(renderer, &rect);
    if (!raw_surf) return;

    /* 4. Estandarizamos los colores a 32 bits y los copiamos a la RAM de tu programa en C */
    SDL_Surface* surf = SDL_ConvertSurface(raw_surf, SDL_PIXELFORMAT_ARGB8888);
    SDL_DestroySurface(raw_surf);
    if (!surf) return;

    memcpy(pixel_data, surf->pixels, w * h * 4);
    SDL_DestroySurface(surf);
}

void putimage(int left, int top, void *bitmap, int op) {
    if (!bitmap) return;
    
    /* 1. Leemos las dimensiones desde el encabezado */
    int *header = (int *)bitmap;
    int w = header[0];
    int h = header[1];
    if (w <= 0 || h <= 0) return;

    /* 2. Apuntamos a donde empiezan los colores reales */
    void *pixel_data = (char *)bitmap + 8;

    /* 3. Reconstruimos una superficie a partir de la memoria bruta */
    SDL_Surface* surf = SDL_CreateSurfaceFrom(w, h, SDL_PIXELFORMAT_ARGB8888, pixel_data, w * 4);
    if (!surf) return;

    /* 4. Subimos la superficie a la VRAM y la pegamos en la pantalla */
    SDL_Texture* tex = SDL_CreateTextureFromSurface(renderer, surf);
    if (tex) {
        SDL_FRect dest = { (float)left, (float)top, (float)w, (float)h };
        SDL_RenderTexture(renderer, tex, NULL, &dest);
        SDL_RenderPresent(renderer);
        SDL_DestroyTexture(tex);
    }
    SDL_DestroySurface(surf);
}

/* --- INTERFAZ GRÁFICA (UI) --- */

int button(int left, int top, int right, int bottom, const char *text) {
    /* 1. Dibujamos el aspecto visual del botón */
    int old_fill = current_fill_color;
    int old_color = current_color;

    /* Fondo */
    setfillstyle(SOLID_FILL, LIGHTGRAY);
    bar(left, top, right, bottom);
    
    /* Borde superior/izquierdo claro (Efecto relieve 3D) */
    setcolor(WHITE); 
    line(left, top, right, top);
    line(left, top, left, bottom);
    
    /* Borde inferior/derecho oscuro (Sombra) */
    setcolor(DARKGRAY); 
    line(right, bottom, right, top);
    line(right, bottom, left, bottom);

    /* Texto centrado */
    setcolor(BLACK);
    int cx = left + ((right - left) / 2);
    int cy = top + ((bottom - top) / 2);
    
    int old_horiz = current_text_horiz;
    int old_vert = current_text_vert;
    settextjustify(CENTER_TEXT, CENTER_TEXT);
    
    outtextxy(cx, cy, text);
    
    /* Restauramos los ajustes que tenía el usuario antes de llamar al botón */
    settextjustify(old_horiz, old_vert);
    setfillstyle(SOLID_FILL, old_fill);
    setcolor(old_color);

    /* 2. Lógica matemática de colisión (El corazón de la función) */
    if (ismouseclick()) {
        if (mouse_x >= left && mouse_x <= right && mouse_y >= top && mouse_y <= bottom) {
            return 1; /* ¡Se hizo clic dentro del cuadro! */
        }
    }
    
    return 0; /* No se hizo clic, o el clic fue fuera de este botón */
}