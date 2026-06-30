#ifndef LIBGRAPH3_H
#define LIBGRAPH3_H

/* Si el compilador es de C++ (g++), activamos el bloque extern "C" */
#ifdef __cplusplus
extern "C" {
#endif

#define DETECT 0

/* Paleta clásica de BGI */
#define BLACK 0
#define BLUE 1
#define GREEN 2
#define CYAN 3
#define RED 4
#define MAGENTA 5
#define BROWN 6
#define LIGHTGRAY 7
#define DARKGRAY 8
#define LIGHTBLUE 9
#define LIGHTGREEN 10
#define LIGHTCYAN 11
#define LIGHTRED 12
#define LIGHTMAGENTA 13
#define YELLOW 14
#define WHITE 15

/* Estilos de Relleno */
#define EMPTY_FILL 0
#define SOLID_FILL 1

/* Macros para settextjustify */
#define LEFT_TEXT    0
#define CENTER_TEXT  1
#define RIGHT_TEXT   2
#define BOTTOM_TEXT  0
#define TOP_TEXT     2

/* Memoria Visual y Sprites */
#define COPY_PUT 0

/* Inicialización */
void initgraph(int *graphdriver, int *graphmode, const char *pathtodriver);
void closegraph(void);
void delay(int millis);

/* Geometría Avanzada */
void drawpoly(int numpoints, int *polypoints);
void ellipse(int x, int y, int stangle, int endangle, int xradius, int yradius);
void arc(int x, int y, int stangle, int endangle, int radius);

/* Nuevas primitivas de dibujo */
void setcolor(int color);
void setbkcolor(int color);
void cleardevice(void);
void line(int x1, int y1, int x2, int y2);

/* Geometría */
void putpixel(int x, int y, int color);
void rectangle(int left, int top, int right, int bottom);
void circle(int x, int y, int radius);

/* Figuras con Relleno */
void setfillstyle(int pattern, int color);
void bar(int left, int top, int right, int bottom);

/* Figuras con Relleno */
void setfillstyle(int pattern, int color);
void bar(int left, int top, int right, int bottom);
void floodfill(int x, int y, int border);

/*Cursor Gráfico */
void moveto(int x, int y);
void lineto(int x, int y);
void moverel(int dx, int dy);
void linerel(int dx, int dy);

/* Interacción y Eventos */
int kbhit(void);
int getch(void);

/* Texto moderno */
void outtextxy(int x, int y, const char *text);

int getmaxx(void);
int getmaxy(void);
int textwidth(const char *text);
int textheight(const char *text);

/* Lectores de Estado (Getters) */
int getx(void);
int gety(void);
int getcolor(void);
int getbkcolor(void);

/* Funciones de Texto Dinámico */
void settextstyle(int font, int direction, int charsize);
void settextjustify(int horiz, int vert);

unsigned int imagesize(int left, int top, int right, int bottom);
void getimage(int left, int top, int right, int bottom, void *bitmap);
void putimage(int left, int top, void *bitmap, int op);

/* Soporte para Ratón y UI */
int ismouseclick(void);
int mousex(void);
int mousey(void);
int button(int left, int top, int right, int bottom, const char *text);

/* Cerramos el bloque extern "C" si estamos en C++ */
#ifdef __cplusplus
}
#endif

#endif