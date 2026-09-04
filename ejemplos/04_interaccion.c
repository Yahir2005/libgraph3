#include <libgraph3.h>
#include <stdio.h>

int main() {
    initwindow(640, 480, "04 - Interaccion y Eventos");
    
    int x = 320, y = 240;
    
    outtextxy(10, 10, "Mueve el circulo con las FLECHAS del teclado.");
    outtextxy(10, 30, "Haz CLIC para teletransportarlo. ESC para salir.");

    while (1) {
        /* kbhit no bloquea la ejecucion, devuelve 1 si hay un evento pendiente */
        if (kbhit()) {
            int tecla = getch();
            
            /* Tecla ESC para salir */
            if (tecla == 27) break; 
            
            /* Controles basicos de teclado (usando codigos ASCII o equivalentes de flechas en SDL) */
            /* En esta libreria, las flechas SDL se traducen o se puede usar WASD para asegurar */
            if (tecla == 'w' || tecla == 'W') y -= 10;
            if (tecla == 's' || tecla == 'S') y += 10;
            if (tecla == 'a' || tecla == 'A') x -= 10;
            if (tecla == 'd' || tecla == 'D') x += 10;
        }

        /* Comprobando eventos del raton */
        if (ismouseclick()) {
            x = mousex();
            y = mousey();
            clearmouseclick(); /* Fundamental limpiar el clic para no detectarlo doble */
        }

        /* Renderizar fotograma */
        cleardevice();
        setcolor(WHITE);
        outtextxy(10, 10, "Usa WASD para moverte. CLIC para saltar. ESC para salir.");
        
        setcolor(YELLOW);
        setfillstyle(SOLID_FILL, RED);
        circle(x, y, 20);
        floodfill(x, y, YELLOW);

        /* Controlar la velocidad del bucle (60 fps aprox) */
        delay(16);
    }

    closegraph();
    return 0;
}
