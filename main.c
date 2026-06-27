#include "libgraph3.h"

int main() {
    int gd = DETECT, gm = 0;
    initgraph(&gd, &gm, "");

    setbkcolor(LIGHTGRAY); /* Un fondo gris clásico */
    cleardevice();

    /* 1. Configuramos el estilo de relleno para hacer un bloque azul */
    setfillstyle(SOLID_FILL, BLUE);
    
    /* 2. Dibujamos la barra sólida (El interior) */
    bar(200, 200, 440, 280);

    /* 3. Le dibujamos un borde blanco brillante por fuera (Efecto 3D / Botón) */
    setcolor(WHITE);
    rectangle(198, 198, 442, 282);
    rectangle(199, 199, 441, 281); /* Doble línea para darle grosor */

    /* 4. Ponemos un texto centrado encima del relleno */
    setcolor(YELLOW);
    const char *mensaje = "BOTON DE INICIO";
    
    int cx = 200 + ((440 - 200) / 2); /* Centro X del rectángulo */
    int cy = 200 + ((280 - 200) / 2); /* Centro Y del rectángulo */
    
    int text_x = cx - (textwidth(mensaje) / 2);
    int text_y = cy - (textheight(mensaje) / 2);
    
    outtextxy(text_x, text_y, mensaje);

    getch(); 
    closegraph();
    
    return 0;
}