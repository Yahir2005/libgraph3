#include <libgraph3.h>

int main() {
    int gd = DETECT, gm = 0;
    initgraph(&gd, &gm, "");

    setbkcolor(DARKGRAY);
    cleardevice();

    /* Calculamos el centro usando nuestros medidores de la fase anterior */
    int cx = getmaxx() / 2;
    int cy = getmaxy() / 2;

    /* Dibujaremos una forma continua con lineto() */
    setcolor(LIGHTGREEN);
    
    /* 1. Apoyamos el lápiz en la punta superior */
    moveto(cx, cy - 100); 
    
    /* 2. Empezamos a trazar sin levantarlo */
    lineto(cx + 40, cy - 20);
    lineto(cx + 120, cy - 20);
    lineto(cx + 50, cy + 30);
    lineto(cx + 70, cy + 110);
    lineto(cx, cy + 60);
    lineto(cx - 70, cy + 110);
    lineto(cx - 50, cy + 30);
    lineto(cx - 120, cy - 20);
    lineto(cx - 40, cy - 20);
    lineto(cx, cy - 100); /* Cerramos la figura regresando al inicio */

    /* Agregamos algo de texto abajo */
    setcolor(WHITE);
    outtextxy(cx - 120, cy + 140, "Un trazo continuo usando lineto()");

    getch(); 
    closegraph();
    
    return 0;
}