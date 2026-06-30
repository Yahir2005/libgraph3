#include "libgraph3.h"
#include <stdio.h>

int main() {
    int gd = DETECT, gm = 0;
    initgraph(&gd, &gm, "");

    setbkcolor(DARKGRAY);
    cleardevice();

    settextstyle(0, 0, 1.5);

    /* Bucle infinito de la interfaz (Game Loop) */
    while (!kbhit()) {
        
        /* Si la función devuelve 1 (clic detectado), entra al bloque.
           Las coordenadas podrían venir directamente de lo que dibujaste en VS Code
        */
        if (button(200, 200, 440, 260, "EJECUTAR ACCION")) {
            
            /* AQUÍ ADENTRO VA EL CÓDIGO QUE EL USUARIO QUIERE EJECUTAR */
            setcolor(YELLOW);
            outtextxy(320, 320, "¡El codigo del boton se ha ejecutado!");
            
            /* Hacemos una pequeña pausa para que el texto se alcance a leer */
            delay(1000); 
            
            /* Limpiamos para el siguiente clic */
            cleardevice(); 
        }
        
        delay(16); /* Da respiro al CPU (60 FPS) */
    }

    closegraph();
    return 0;
}