#include <libgraph3.h>
#include <stdlib.h>

int main() {
    initwindow(800, 600, "06 - Geometria Avanzada e Imagenes");

    setcolor(WHITE);
    outtextxy(10, 10, "Dibujando un poligono (Estrella)");

    /* Coordenadas de un poligono x,y secuenciales */
    int estrella[] = {
        150, 50,  180, 140, 280, 140,
        200, 200, 230, 290, 150, 240,
        70,  290, 100, 200, 20,  140,
        120, 140, 150, 50
    };
    /* drawpoly toma el numero de PUNTOS (pares x,y), asi que 11 puntos */
    setcolor(YELLOW);
    drawpoly(11, estrella);

    /* Arco y Elipse */
    setcolor(LIGHTMAGENTA);
    ellipse(500, 150, 0, 360, 100, 50);
    
    setcolor(LIGHTCYAN);
    arc(500, 300, 45, 135, 100);

    /* Manejo de Memoria de Imagen (getimage / putimage) */
    outtextxy(10, 320, "Copiando la estrella con getimage/putimage");
    unsigned int tam = imagesize(20, 50, 280, 290);
    void* buffer = malloc(tam);
    
    getimage(20, 50, 280, 290, buffer);
    putimage(400, 400, buffer, COPY_PUT);
    free(buffer);

    /* Nota: para probar readimagefile, descomenta lo siguiente y pon una imagen BMP en la carpeta */
    /* readimagefile("prueba.bmp", 0, 0, 100, 100); */

    outtextxy(10, 550, "Presiona una tecla para terminar.");
    getch();

    closegraph();
    return 0;
}
