#include "libgraph3.h"
#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <stdio.h>
#include <math.h> 
#include <stdlib.h> 

#define PI 3.14159265358979323846

static SDL_Window* window = NULL;
static SDL_Renderer* renderer = NULL;
static TTF_Font* default_font = NULL;

static int current_color = WHITE;
static int current_bkcolor = BLACK;

static int cp_x = 0; 
static int cp_y = 0; 

static int key_buffer = 0;

static int current_fill_color = WHITE; 
static int current_fill_pattern = SOLID_FILL; 

static int current_text_horiz = LEFT_TEXT;
static int current_text_vert = TOP_TEXT;
static int current_text_size = 18; 

/* Estado del ratón */
static int mouse_x = 0;
static int mouse_y = 0;
static int mouse_clicked = 0;
static int mouse_down = 0;

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

    if (!TTF_Init()) {
        printf("Error inicializando SDL_ttf: %s\n", SDL_GetError());
        return;
    }

    default_font = TTF_OpenFont("/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf", 18);
    if (!default_font) {
        printf("Advertencia: No se pudo cargar la fuente por defecto.\n");
    }

    cleardevice();
}

void closegraph(void) {
    if (default_font) TTF_CloseFont(default_font);
    TTF_Quit(); 
    if (renderer) SDL_DestroyRenderer(renderer);
    if (window) SDL_DestroyWindow(window);
    SDL_Quit();
}

void delay(int millis) {
    /* Aqui se hace visible el frame completo: todas las primitivas dibujadas
       desde el ultimo present() se publican de una sola vez, evitando el
       parpadeo / dibujos a medias que se veian antes. */
    SDL_RenderPresent(renderer);
    SDL_Delay(millis);
}

/* --- INTERACCIÓN Y EVENTOS --- */

static void procesar_eventos(void) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_EVENT_QUIT) {
            key_buffer = 27; /* ESC */
        } else if (event.type == SDL_EVENT_KEY_DOWN) {
            key_buffer = (int)event.key.key;
        } else if (event.type == SDL_EVENT_MOUSE_BUTTON_DOWN) {
            mouse_x = (int)event.button.x;
            mouse_y = (int)event.button.y;
            mouse_clicked = 1; 
        }
    }
}

int kbhit(void) {
    SDL_RenderPresent(renderer);
    procesar_eventos();
    return (key_buffer != 0) ? 1 : 0;
}

int getch(void) {
    SDL_RenderPresent(renderer);
    while (key_buffer == 0) {
        procesar_eventos();
        SDL_Delay(10); 
    }
    int key = key_buffer;
    key_buffer = 0; 
    return key;
}

int ismouseclick(void) {

    return mouse_clicked;
}

void clearmouseclick(void) {
    mouse_clicked = 0; 
}

int ismousedown(void) {
    float live_x, live_y;
    /* Extrae el estado actual de los botones y las coordenadas sin tocar la cola de eventos */
    SDL_MouseButtonFlags estado_botones = SDL_GetMouseState(&live_x, &live_y);
    
    /* Si el botón izquierdo (L) está presionado, actualizamos la memoria */
    if (estado_botones & SDL_BUTTON_LMASK) {
        mouse_x = (int)live_x;
        mouse_y = (int)live_y;
        return 1;
    }
    
    return 0;
}

int mousex(void) { return mouse_x; }
int mousey(void) { return mouse_y; }

void setcolor(int color) { if (color >= 0 && color <= 15) current_color = color; }
void setbkcolor(int color) { if (color >= 0 && color <= 15) current_bkcolor = color; }

void cleardevice(void) {
    BGIColor bg = palette[current_bkcolor];
    SDL_SetRenderDrawColor(renderer, bg.r, bg.g, bg.b, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(renderer);
    /* No presentamos aqui: solo preparamos el back buffer.
       El frame se hace visible una sola vez por ciclo, en delay()/getch()/kbhit(). */

    cp_x = 0;
    cp_y = 0;
}

void line(int x1, int y1, int x2, int y2) {
    BGIColor fg = palette[current_color];
    SDL_SetRenderDrawColor(renderer, fg.r, fg.g, fg.b, SDL_ALPHA_OPAQUE);
    SDL_RenderLine(renderer, x1, y1, x2, y2);
}

void putpixel(int x, int y, int color) {
    int old_color = current_color;
    setcolor(color);
    BGIColor fg = palette[current_color];
    SDL_SetRenderDrawColor(renderer, fg.r, fg.g, fg.b, SDL_ALPHA_OPAQUE);
    SDL_RenderPoint(renderer, (float)x, (float)y);
    setcolor(old_color);
}

void rectangle(int left, int top, int right, int bottom) {
    BGIColor fg = palette[current_color];
    SDL_SetRenderDrawColor(renderer, fg.r, fg.g, fg.b, SDL_ALPHA_OPAQUE);
    SDL_FRect rect = { (float)left, (float)top, (float)(right - left), (float)(bottom - top) };
    SDL_RenderRect(renderer, &rect);
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
}

/* --- LA NUEVA FUNCIÓN DE TEXTO --- */
void outtextxy(int x, int y, const char *text) {
    if (!default_font || !text) return;

    int text_w = 0, text_h = 0;
    TTF_GetStringSize(default_font, text, 0, &text_w, &text_h);

    int adj_x = x;
    if (current_text_horiz == CENTER_TEXT) {
        adj_x = x - (text_w / 2);
    } else if (current_text_horiz == RIGHT_TEXT) {
        adj_x = x - text_w;
    }

    int adj_y = y;
    if (current_text_vert == CENTER_TEXT) {
        adj_y = y - (text_h / 2);
    } else if (current_text_vert == BOTTOM_TEXT) {
        adj_y = y - text_h;
    }

    BGIColor fg = palette[current_color];
    SDL_Color textColor = { fg.r, fg.g, fg.b, 255 };

    SDL_Surface* textSurface = TTF_RenderText_Blended(default_font, text, 0, textColor);
    if (!textSurface) return;

    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    SDL_FRect renderQuad = { (float)adj_x, (float)adj_y, (float)textSurface->w, (float)textSurface->h };

    SDL_RenderTexture(renderer, textTexture, NULL, &renderQuad);

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
    cp_x = x;
    cp_y = y;
}

void lineto(int x, int y) {
    line(cp_x, cp_y, x, y);
    cp_x = x;
    cp_y = y;
}

void moverel(int dx, int dy) {
    cp_x += dx;
    cp_y += dy;
}

void linerel(int dx, int dy) {
    line(cp_x, cp_y, cp_x + dx, cp_y + dy);
    cp_x += dx;
    cp_y += dy;
}

/* --- GEOMETRÍA AVANZADA --- */

void drawpoly(int numpoints, int *polypoints) {
    if (numpoints < 2 || !polypoints) return;
    
    for (int i = 0; i < numpoints - 1; i++) {
        line(polypoints[i * 2], polypoints[i * 2 + 1],
             polypoints[(i + 1) * 2], polypoints[(i + 1) * 2 + 1]);
    }
}

void ellipse(int x, int y, int stangle, int endangle, int xradius, int yradius) {
    if (xradius <= 0 || yradius <= 0) return;

    BGIColor fg = palette[current_color];
    SDL_SetRenderDrawColor(renderer, fg.r, fg.g, fg.b, SDL_ALPHA_OPAQUE);

    float step = 1.0f / (float)(xradius > yradius ? xradius : yradius);
    
    float start_rad = stangle * PI / 180.0f;
    float end_rad = endangle * PI / 180.0f;

    if (end_rad < start_rad) end_rad += 2.0f * PI;

    float last_px = x + xradius * cosf(start_rad);
    float last_py = y - yradius * sinf(start_rad); 

    for (float theta = start_rad + step; theta <= end_rad; theta += step) {
        float px = x + xradius * cosf(theta);
        float py = y - yradius * sinf(theta);
        SDL_RenderLine(renderer, last_px, last_py, px, py);
        last_px = px;
        last_py = py;
    }
    
    float final_px = x + xradius * cosf(end_rad);
    float final_py = y - yradius * sinf(end_rad);
    SDL_RenderLine(renderer, last_px, last_py, final_px, final_py);
}

void arc(int x, int y, int stangle, int endangle, int radius) {
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
    if (current_fill_pattern == EMPTY_FILL) return;

    BGIColor fill_bg = palette[current_fill_color];
    SDL_SetRenderDrawColor(renderer, fill_bg.r, fill_bg.g, fill_bg.b, SDL_ALPHA_OPAQUE);

    SDL_FRect rect = { (float)left, (float)top, (float)(right - left), (float)(bottom - top) };
    
    SDL_RenderFillRect(renderer, &rect);
}

void floodfill(int x, int y, int border) {
    SDL_Surface* raw_surf = SDL_RenderReadPixels(renderer, NULL);
    if (!raw_surf) return;

    SDL_Surface* surf = SDL_ConvertSurface(raw_surf, SDL_PIXELFORMAT_ARGB8888);
    SDL_DestroySurface(raw_surf);
    if (!surf) return;

    int w = surf->w;
    int h = surf->h;

    if (x < 0 || x >= w || y < 0 || y >= h) {
        SDL_DestroySurface(surf);
        return;
    }

    BGIColor fill_bgi = palette[current_fill_color];
    Uint32 fill_color = (255 << 24) | (fill_bgi.r << 16) | (fill_bgi.g << 8) | fill_bgi.b;

    BGIColor border_bgi = palette[border];
    Uint32 target_border = (255 << 24) | (border_bgi.r << 16) | (border_bgi.g << 8) | border_bgi.b;

    Uint32* pixels = (Uint32*)surf->pixels;
    Uint32 start_color = pixels[y * w + x];

    if (start_color == target_border || start_color == fill_color) {
        SDL_DestroySurface(surf);
        return;
    }

    int max_stack = w * h * 4; 
    int* stack_x = (int*)malloc(max_stack * sizeof(int));
    int* stack_y = (int*)malloc(max_stack * sizeof(int));
    int stack_ptr = 0;

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

        if (current_pixel != target_border && current_pixel != fill_color) {
            pixels[idx] = fill_color;
            
            stack_x[stack_ptr] = cx + 1; stack_y[stack_ptr] = cy; stack_ptr++;
            stack_x[stack_ptr] = cx - 1; stack_y[stack_ptr] = cy; stack_ptr++;
            stack_x[stack_ptr] = cx; stack_y[stack_ptr] = cy + 1; stack_ptr++;
            stack_x[stack_ptr] = cx; stack_y[stack_ptr] = cy - 1; stack_ptr++;
        }
    }

    free(stack_x);
    free(stack_y);

    SDL_Texture* tex = SDL_CreateTextureFromSurface(renderer, surf);
    SDL_RenderTexture(renderer, tex, NULL, NULL);

    SDL_DestroyTexture(tex);
    SDL_DestroySurface(surf);
}

/* --- TEXTO DINÁMICO --- */

void settextstyle(int font, int direction, int charsize) {
    if (charsize <= 0) charsize = 1;
    current_text_size = charsize * 12; 

    if (default_font) {
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
    
    return 8 + (w * h * 4);
}

void getimage(int left, int top, int right, int bottom, void *bitmap) {
    if (!bitmap) return;
    
    int w = right - left;
    int h = bottom - top;
    if (w <= 0 || h <= 0) return;

    int *header = (int *)bitmap;
    header[0] = w;
    header[1] = h;

    Uint32 *pixel_data = (Uint32 *)((char *)bitmap + 8);

    SDL_Rect rect = { left, top, w, h };
    SDL_Surface* raw_surf = SDL_RenderReadPixels(renderer, &rect);
    if (!raw_surf) return;

    SDL_Surface* surf = SDL_ConvertSurface(raw_surf, SDL_PIXELFORMAT_ARGB8888);
    SDL_DestroySurface(raw_surf);
    if (!surf) return;

    memcpy(pixel_data, surf->pixels, w * h * 4);
    SDL_DestroySurface(surf);
}

void putimage(int left, int top, void *bitmap, int op) {
    if (!bitmap) return;
    
    int *header = (int *)bitmap;
    int w = header[0];
    int h = header[1];
    if (w <= 0 || h <= 0) return;

    void *pixel_data = (char *)bitmap + 8;

    SDL_Surface* surf = SDL_CreateSurfaceFrom(w, h, SDL_PIXELFORMAT_ARGB8888, pixel_data, w * 4);
    if (!surf) return;

    SDL_Texture* tex = SDL_CreateTextureFromSurface(renderer, surf);
    if (tex) {
        SDL_FRect dest = { (float)left, (float)top, (float)w, (float)h };
        SDL_RenderTexture(renderer, tex, NULL, &dest);
        SDL_DestroyTexture(tex);
    }
    SDL_DestroySurface(surf);
}

/* --- INTERFAZ GRÁFICA (UI) --- */

int button(int left, int top, int right, int bottom, const char *text) {
    int old_fill = current_fill_color;
    int old_color = current_color;

    setfillstyle(SOLID_FILL, LIGHTGRAY);
    bar(left, top, right, bottom);
    
    setcolor(WHITE); 
    line(left, top, right, top);
    line(left, top, left, bottom);
    
    setcolor(DARKGRAY); 
    line(right, bottom, right, top);
    line(right, bottom, left, bottom);

    setcolor(BLACK);
    int cx = left + ((right - left) / 2);
    int cy = top + ((bottom - top) / 2);
    
    int old_horiz = current_text_horiz;
    int old_vert = current_text_vert;
    settextjustify(CENTER_TEXT, CENTER_TEXT);
    
    outtextxy(cx, cy, text);
    
    settextjustify(old_horiz, old_vert);
    setfillstyle(SOLID_FILL, old_fill);
    setcolor(old_color);

    if (ismouseclick()) {
        if (mouse_x >= left && mouse_x <= right && mouse_y >= top && mouse_y <= bottom) {
            clearmouseclick();
            return 1;
        }
    }
    
    return 0; 
}

int checkbox(int x, int y, const char *text, int *estado) {
    int w = 16, h = 16;
    int old_color = current_color;
    int old_fill = current_fill_color;
    int old_horiz = current_text_horiz;
    int old_vert = current_text_vert;

    // Dibujar la caja
    setfillstyle(SOLID_FILL, WHITE);
    bar(x, y, x + w, y + h);

    setcolor(DARKGRAY);
    line(x, y, x + w, y);
    line(x, y, x, y + h);
    setcolor(LIGHTGRAY);
    line(x + w, y + h, x + w, y);
    line(x + w, y + h, x, y + h);

    if (*estado == 1) {
        setcolor(BLACK);
        line(x + 3, y + 3, x + w - 3, y + h - 3);
        line(x + 3, y + h - 3, x + w - 3, y + 3);
    }

    setcolor(WHITE);
    settextjustify(LEFT_TEXT, TOP_TEXT);
    outtextxy(x + w + 8, y + 2, text);

    // --- ÁREA DE DETECCIÓN MEJORADA ---
    int text_w = textwidth(text);
    int text_h = textheight(text);
    // Cubrimos toda la altura del texto (o la caja, la que sea mayor)
    int click_w = w + 8 + text_w;
    int click_h = (text_h > h) ? text_h : h;

    if (ismouseclick()) {
        if (mouse_x >= x && mouse_x <= x + click_w &&
            mouse_y >= y && mouse_y <= y + click_h) {
            *estado = !(*estado);
            clearmouseclick();
            return 1;
        }
    }

    // Restaurar
    setcolor(old_color);
    setfillstyle(SOLID_FILL, old_fill);
    settextjustify(old_horiz, old_vert);
    return 0;
}

/* --- CUADROS DE DIÁLOGO (UI) --- */

int inputdialog_int(const char *prompt) {
    int cx = getmaxx() / 2;
    int cy = getmaxy() / 2;
    int w = 320, h = 140;
    int left = cx - w/2, top = cy - h/2, right = cx + w/2, bottom = cy + h/2;

    unsigned int bg_size = imagesize(left, top, right, bottom);
    void *bg_saved = NULL;
    if (bg_size > 0) {
        bg_saved = malloc(bg_size);
        if (bg_saved) getimage(left, top, right, bottom, bg_saved);
    }

    char input_buffer[20] = ""; 
    int buf_len = 0;
    int typing = 1;

    int old_fill = current_fill_color;
    int old_color = current_color;
    int old_horiz = current_text_horiz;
    int old_vert = current_text_vert;

    settextjustify(CENTER_TEXT, CENTER_TEXT);

    while (typing) {
        setfillstyle(SOLID_FILL, LIGHTGRAY);
        bar(left, top, right, bottom);
        
        /* Bordes 3D corregidos */
        setcolor(WHITE);
        line(left, top, right - 1, top); line(left, top, left, bottom - 1);
        setcolor(DARKGRAY);
        line(right - 1, bottom - 1, right - 1, top); line(right - 1, bottom - 1, left, bottom - 1);

        setcolor(BLACK);
        outtextxy(cx, cy - 30, prompt);

        setfillstyle(SOLID_FILL, WHITE);
        bar(cx - 100, cy + 5, cx + 100, cy + 35);
        setcolor(DARKGRAY);
        line(cx - 100, cy + 5, cx + 100, cy + 5); line(cx - 100, cy + 5, cx - 100, cy + 35);

        setcolor(BLACK);
        outtextxy(cx, cy + 20, input_buffer);

        int key = getch(); 

        if (key == '\r' || key == '\n' || key == 13) {
            typing = 0; 
        } 
        else if (key == '\b' || key == 8) { 
            if (buf_len > 0) {
                buf_len--;
                input_buffer[buf_len] = '\0';
            }
        } 
        else if (buf_len < 18) {
            if ((key >= '0' && key <= '9') || (key == '-' && buf_len == 0)) {
                input_buffer[buf_len] = (char)key;
                buf_len++;
                input_buffer[buf_len] = '\0'; 
            }
        }
    }

    if (bg_saved) {
        putimage(left, top, bg_saved, COPY_PUT);
        free(bg_saved);
    }

    settextjustify(old_horiz, old_vert);
    setfillstyle(SOLID_FILL, old_fill);
    setcolor(old_color);

    return atoi(input_buffer);
}

void progressbar(int left, int top, int right, int bottom, int porcentaje) {
    /* 1. Protección contra desbordamientos matemáticos */
    if (porcentaje < 0) porcentaje = 0;
    if (porcentaje > 100) porcentaje = 100;

    int old_color = current_color;
    int old_fill = current_fill_color;
    int old_horiz = current_text_horiz;
    int old_vert = current_text_vert;

    /* 2. Dibujamos el "carril" vacío de la barra (Gris oscuro) */
    setfillstyle(SOLID_FILL, DARKGRAY);
    bar(left, top, right, bottom);

    /* Bordes estilo hundido (3D inverso) */
    setcolor(LIGHTGRAY);
    line(left, bottom, right, bottom);
    line(right, top, right, bottom);
    setcolor(BLACK);
    line(left, top, right, top);
    line(left, top, left, bottom);

    /* 3. Calculamos y dibujamos el relleno (Verde) */
    if (porcentaje > 0) {
        /* Calculamos el ancho disponible quitando 2 píxeles de los bordes */
        int ancho_total = (right - left) - 2;
        int ancho_relleno = (ancho_total * porcentaje) / 100;
        
        setfillstyle(SOLID_FILL, LIGHTGREEN);
        bar(left + 1, top + 1, left + 1 + ancho_relleno, bottom - 1);
    }

    /* 4. Dibujamos el texto del porcentaje centrado */
    char texto_porcentaje[10];
    sprintf(texto_porcentaje, "%d%%", porcentaje);
    
    int cx = left + ((right - left) / 2);
    int cy = top + ((bottom - top) / 2);
    
    /* Si el relleno ya pasó la mitad, el texto se vuelve negro para contrastar con el verde */
    if (porcentaje > 50) {
        setcolor(BLACK);
    } else {
        setcolor(WHITE);
    }

    settextjustify(CENTER_TEXT, CENTER_TEXT);
    outtextxy(cx, cy, texto_porcentaje);

    /* 5. Restauramos el entorno */
    setcolor(old_color);
    setfillstyle(SOLID_FILL, old_fill);
    settextjustify(old_horiz, old_vert);
}

int slider(int x, int y, int len, const char *text, int *valor) {
    /* Protección matemática para la variable en memoria */
    if (*valor < 0) *valor = 0;
    if (*valor > 100) *valor = 100;

    int old_color = current_color;
    int old_fill = current_fill_color;
    int old_horiz = current_text_horiz;
    int old_vert = current_text_vert;

    /* Dibujamos el carril (riel) del slider */
    setcolor(DARKGRAY);
    line(x, y + 10, x + len, y + 10);
    setcolor(WHITE);
    line(x, y + 11, x + len, y + 11);

    /* Calculamos la coordenada X de la manija basada en el valor actual */
    int thumb_x = x + (len * (*valor)) / 100;

    /* Dibujamos la manija (Thumb) */
    setfillstyle(SOLID_FILL, LIGHTGRAY);
    bar(thumb_x - 5, y, thumb_x + 5, y + 20);
    
    /* Bordes 3D de la manija */
    setcolor(WHITE); 
    line(thumb_x - 5, y, thumb_x + 5, y); 
    line(thumb_x - 5, y, thumb_x - 5, y + 20);
    setcolor(DARKGRAY); 
    line(thumb_x - 5, y + 20, thumb_x + 5, y + 20); 
    line(thumb_x + 5, y, thumb_x + 5, y + 20);

    /* Texto descriptivo a la derecha */
    setcolor(WHITE);
    settextjustify(LEFT_TEXT, TOP_TEXT);
    outtextxy(x + len + 15, y + 2, text);

    /* --- LÓGICA DE ARRASTRE (DRAG) --- */
    /* Utilizamos nuestro nuevo método independiente */
    if (ismousedown()) {
        /* Ampliamos el área de detección ('hitbox') para que sea fácil de agarrar */
        if (mousex() >= x && mousex() <= x + len && 
            mousey() >= y - 10 && mousey() <= y + 30) {
            
            /* Regla de tres simple para convertir pixeles a un porcentaje (0-100) */
            float proporcion = (float)(mousex() - x) / (float)len;
            *valor = (int)(proporcion * 100.0f);
            
            if (*valor < 0) *valor = 0;
            if (*valor > 100) *valor = 100;
            
            /* Retornamos 1 indicando que el slider fue manipulado en este fotograma */
            return 1; 
        }
    }

    /* Restauramos los ajustes visuales */
    setcolor(old_color);
    setfillstyle(SOLID_FILL, old_fill);
    settextjustify(old_horiz, old_vert);

    return 0;
}

int textfield(int x, int y, int width, char *buffer, int max_len, int *is_focused) {
    int h = 30; 
    
    int old_color = current_color;
    int old_fill = current_fill_color;
    int old_horiz = current_text_horiz;
    int old_vert = current_text_vert;

    /* 1. LÓGICA DE FOCO */
    if (ismouseclick()) {
        if (mousex() >= x && mousex() <= x + width && 
            mousey() >= y && mousey() <= y + h) {
            *is_focused = 1; 
            clearmouseclick();
        } else if (*is_focused == 1) {
            *is_focused = 0; 
        }
    }

    /* 2. LÓGICA DE TECLADO (INDEPENDIENTE) */
    /* En vez de llamar a kbhit(), leemos directamente la memoria del motor */
    if (*is_focused) {
        if (key_buffer != 0) {
            int key = key_buffer;
            
            /* Si es ESC (27), no la consumimos para que el bucle principal pueda salir */
            if (key != 27) {
                key_buffer = 0; /* Consumimos la tecla en silencio */
                
                int len = 0;
                while (buffer[len] != '\0') len++;

                /* Retroceso (Backspace) */
                if ((key == '\b' || key == 8) && len > 0) {
                    buffer[len - 1] = '\0';
                } 
                /* Caracteres imprimibles */
                else if (key >= 32 && key <= 126 && len < max_len - 1) {
                    buffer[len] = (char)key;
                    buffer[len + 1] = '\0'; 
                }
            }
        }
    }

    /* 3. DIBUJADO DEL COMPONENTE */
    setfillstyle(SOLID_FILL, WHITE);
    bar(x, y, x + width, y + h);

    if (*is_focused) {
        setcolor(BLUE);
    } else {
        setcolor(DARKGRAY);
    }
    
    line(x, y, x + width, y);
    line(x, y, x, y + h);
    line(x, y + h, x + width, y + h);
    line(x + width, y, x + width, y + h);

    /* 4. DIBUJADO DEL TEXTO */
    setcolor(BLACK);
    settextjustify(LEFT_TEXT, CENTER_TEXT);
    outtextxy(x + 8, y + (h / 2), buffer);

    /* 5. CURSOR PARPADEANTE */
    if (*is_focused) {
        int text_w = textwidth(buffer);
        if ((SDL_GetTicks() / 500) % 2 == 0) {
            setcolor(BLACK);
            line(x + 8 + text_w + 2, y + 6, x + 8 + text_w + 2, y + h - 6);
        }
    }

    /* Restauramos entorno */
    setcolor(old_color);
    setfillstyle(SOLID_FILL, old_fill);
    settextjustify(old_horiz, old_vert);

    return *is_focused;
}