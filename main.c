#include <libgraph3.h>

int main() {
    int gd = DETECT, gm = 0;
    initgraph(&gd, &gm, "");

    int x = 50;
    int direccion = 5; /* Velocidad a la que se mueve */

    /* Bucle infinito de animación. Solo se rompe si presionas una tecla */
    while (!kbhit()) {
        /* 1. Borramos el fotograma anterior */
        setbkcolor(BLACK);
        cleardevice();

        /* 2. Dibujamos el nuevo fotograma */
        setcolor(LIGHTCYAN);
        circle(x, 240, 40);
        
        setcolor(WHITE);
        outtextxy(150, 400, "Animacion corriendo... Presiona una tecla para detener.");

        /* 3. Calculamos la física para el siguiente fotograma */
        x += direccion;
        
        /* Si choca contra los bordes, rebota */
        if (x > getmaxx() - 40 || x < 40) {
            direccion = -direccion;
        }

        /* 4. Le damos un respiro al procesador (aproximadamente 60 FPS) */
        delay(16); 
    }

    /* Recogemos la tecla que detuvo el bucle para limpiar el buzón */
    getch(); 

    closegraph();
    return 0;
}