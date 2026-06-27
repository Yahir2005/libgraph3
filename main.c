#include "libgraph3.h"
#include <stdlib.h> /* Para malloc y free */

int main() {
    int gd = DETECT, gm = 0;
    initgraph(&gd, &gm, "");

    /* 1. DIBUJAMOS EL SPRITE (Una pequeña nave o personaje) */
    setbkcolor(BLACK);
    cleardevice();
    
    setfillstyle(SOLID_FILL, LIGHTBLUE);
    bar(10, 10, 40, 40); /* Cuerpo */
    setcolor(YELLOW);
    circle(25, 25, 10);  /* Ojo o ventana */
    line(10, 10, 40, 40);
    line(40, 10, 10, 40);

    /* 2. CAPTURAMOS EL SPRITE A LA MEMORIA */
    /* Calculamos cuánta RAM necesitamos para un bloque de 0,0 a 50,50 */
    unsigned int tamano = imagesize(0, 0, 50, 50);
    
    /* Reservamos la memoria */
    void *mi_sprite = malloc(tamano);
    
    /* Tomamos la foto y la guardamos en mi_sprite */
    getimage(0, 0, 50, 50, mi_sprite);

    /* 3. CREAMOS UN FONDO COMPLEJO PARA DEMOSTRAR QUE NO SE BORRA */
    cleardevice();
    setcolor(DARKGRAY);
    for (int i = 0; i < 100; i++) {
        circle(rand() % getmaxx(), rand() % getmaxy(), rand() % 20);
    }

    /* 4. ANIMAMOS EL SPRITE */
    setcolor(WHITE);
    outtextxy(getmaxx()/2, getmaxy() - 30, "Mueve el raton o usa una animacion automatica. Presiona una tecla para salir.");

    int x = 50, y = 50;
    int dx = 4, dy = 3;

    while (!kbhit()) {
        /* Pegamos la imagen en la coordenada actual */
        putimage(x, y, mi_sprite, COPY_PUT);
        
        delay(16); // ~60 FPS
        
        /* Opcional: Podrías usar getimage antes de poner el sprite para 
           guardar el fondo, y putimage después para restaurarlo, pero 
           para simplificar, dejaremos que deje una "estela" en este demo */
        
        x += dx;
        y += dy;

        /* Rebote en los bordes */
        if (x < 0 || x > getmaxx() - 50) dx = -dx;
        if (y < 0 || y > getmaxy() - 50) dy = -dy;
    }

    getch(); 

    /* 5. LIBERAMOS LA MEMORIA SIEMPRE */
    free(mi_sprite);
    closegraph();
    
    return 0;
}