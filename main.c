#include <libgraph3.h>

int main() {
    int gd = DETECT, gm = 0;
    initgraph(&gd, &gm, "");

    setbkcolor(DARKGRAY);
    cleardevice();

    /* 1. Prueba de drawpoly: Dibujaremos una nave espacial o un rombo */
    /* Nota: Para cerrar la figura, el último punto debe ser igual al primero */
    int puntos_poligono[] = {
        320, 100,  /* Punto 1: Arriba centro */
        400, 250,  /* Punto 2: Derecha */
        320, 300,  /* Punto 3: Abajo centro */
        240, 250,  /* Punto 4: Izquierda */
        320, 100   /* Punto 5: Volvemos a cerrar arriba */
    };
    
    setcolor(LIGHTGREEN);
    drawpoly(5, puntos_poligono); /* 5 puntos totales */

    /* 2. Prueba de ellipse: Un anillo orbital alrededor del polígono */
    setcolor(YELLOW);
    /* Dibuja una elipse completa (0 a 360 grados) */
    ellipse(320, 250, 0, 360, 200, 80);

    /* 3. Prueba de arc: Un arco iris en la parte superior */
    setcolor(LIGHTRED);
    /* Dibuja medio círculo exacto superior (0 a 180 grados) */
    arc(320, 250, 0, 180, 220);

    setcolor(WHITE);
    outtextxy(180, 400, "Poligonos y Trigonometria renderizados");

    getch(); 
    closegraph();
    
    return 0;
}