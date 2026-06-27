#include "libgraph3.h"

int main() {
    int gd = DETECT, gm = 0;
    initgraph(&gd, &gm, "");

    setbkcolor(LIGHTGRAY);
    cleardevice();

    /* 1. Dibujamos 3 círculos amarillos que se cruzan en el medio (Estilo Diagrama de Venn) */
    setcolor(YELLOW);
    circle(280, 220, 80);
    circle(360, 220, 80);
    circle(320, 290, 80);

    /* 2. Seleccionamos el color del relleno (Pintura Roja) */
    setfillstyle(SOLID_FILL, LIGHTRED);
    
    /* 3. Vaciamos la pintura exactamente en la intersección central */
    /* floodfill detectará los bordes amarillos y se detendrá ahí */
    floodfill(320, 240, YELLOW);

    /* Rellenamos los pétalos exteriores de otros colores */
    setfillstyle(SOLID_FILL, LIGHTBLUE);
    floodfill(250, 200, YELLOW);
    
    setfillstyle(SOLID_FILL, LIGHTGREEN);
    floodfill(390, 200, YELLOW);

    setcolor(BLACK);
    outtextxy(180, 420, "Floodfill cruzando el CPU y GPU exitosamente");

    getch(); 
    closegraph();
    
    return 0;
}