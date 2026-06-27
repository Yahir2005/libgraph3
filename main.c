#include "libgraph3.h"

int main() {
    int gd = DETECT, gm = 0;
    initgraph(&gd, &gm, "");

    setbkcolor(DARKGRAY);
    cleardevice();

    int cx = getmaxx() / 2;
    int cy = getmaxy() / 2;

    // Dibujamos un punto de mira central (nuestro eje de coordenadas)
    setcolor(LIGHTRED);
    line(cx - 20, cy, cx + 20, cy);
    line(cx, cy - 20, cx, cy + 20);

    // Prueba 1: Texto grande centrado perfectamente en el eje
    settextstyle(0, 0, 3); // Multiplicador 3 (~36 puntos)
    settextjustify(CENTER_TEXT, CENTER_TEXT);
    setcolor(YELLOW);
    outtextxy(cx, cy, "CENTRADISIMO");

    // Prueba 2: Texto alineado a la derecha arriba del eje
    settextstyle(0, 0, 1.5); // Más pequeño
    settextjustify(RIGHT_TEXT, TOP_TEXT);
    setcolor(LIGHTCYAN);
    outtextxy(cx - 30, cy - 40, "Derecha-Superior ->");

    // Prueba 3: Texto alineado a la izquierda abajo del eje
    settextjustify(LEFT_TEXT, BOTTOM_TEXT);
    setcolor(LIGHTGREEN);
    outtextxy(cx + 30, cy + 40, "<- Izquierda-Inferior");

    getch();
    closegraph();
    return 0;
}