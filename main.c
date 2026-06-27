#include <libgraph3.h>
#include <stdio.h>

int main() {
    int gd = DETECT, gm = 0;
    initgraph(&gd, &gm, "");

    setbkcolor(DARKGRAY);
    cleardevice();

    /* 1. Movemos el lápiz y cambiamos el color */
    setcolor(YELLOW);
    moveto(150, 150);
    lineto(300, 150);
    
    /* 2. Le preguntamos a la librería dónde se quedó y qué color tiene */
    int posicion_x = getx();
    int posicion_y = gety();
    int color_actual = getcolor();
    int fondo_actual = getbkcolor();

    /* 3. Formateamos la información en cadenas de texto */
    char texto_posicion[100];
    char texto_color[100];
    
    sprintf(texto_posicion, "El lapiz se detuvo en: X=%d, Y=%d", posicion_x, posicion_y);
    sprintf(texto_color, "Color de linea: %d | Color de fondo: %d", color_actual, fondo_actual);

    /* 4. Mostramos los datos en pantalla */
    setcolor(WHITE);
    outtextxy(150, 200, texto_posicion);
    outtextxy(150, 230, texto_color);

    /* Un pequeño indicador visual de dónde está el lápiz */
    setcolor(LIGHTRED);
    circle(posicion_x, posicion_y, 5);

    getch(); 
    closegraph();
    
    return 0;
}