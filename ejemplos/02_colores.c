#include <libgraph3.h>

int main() {
    initwindow(640, 480, "02 - Colores y Relleno");

    /* Fondo de pantalla usando color DOS */
    setbkcolor(DARKGRAY);
    cleardevice();

    /* Usando la nueva macro TRUE COLOR para hacer naranja */
    setcolor(COLOR(255, 128, 0));
    rectangle(50, 50, 250, 250);
    
    /* Rellenando el rectangulo con Azul clásico */
    setfillstyle(SOLID_FILL, BLUE);
    floodfill(150, 150, COLOR(255, 128, 0));

    /* Dibujando una barra rellena directamente con True Color */
    setfillstyle(SOLID_FILL, COLOR(0, 255, 128));
    bar(300, 50, 500, 250);

    setcolor(WHITE);
    outtextxy(10, 10, "Colores clasicos y True Color conviviendo. Presiona una tecla.");
    getch();

    closegraph();
    return 0;
}
