#include <libgraph3.h>

int main() {
    /* 1. Inicializar la ventana */
    initwindow(800, 600, "01 - Basico");

    /* 2. Dibujar formas primitivas */
    setcolor(WHITE);
    line(100, 100, 700, 100);

    setcolor(RED);
    rectangle(200, 200, 400, 400);

    setcolor(YELLOW);
    circle(600, 300, 100);

    /* 3. Dibujar un monton de puntos al azar */
    for(int i = 0; i < 1000; i++) {
        putpixel(400 + (i % 200), 100 + (i % 50), LIGHTCYAN);
    }

    /* 4. Esperar a que el usuario presione una tecla */
    outtextxy(10, 10, "Presiona cualquier tecla para salir...");
    getch();

    /* 5. Cerrar el motor grafico */
    closegraph();
    return 0;
}
