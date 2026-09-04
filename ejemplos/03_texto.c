#include <libgraph3.h>
#include <stdio.h>

int main() {
    initwindow(800, 600, "03 - Texto y Fuentes");

    /* Texto basico */
    setcolor(WHITE);
    outtextxy(50, 50, "Hola Mundo desde libgraph3!");

    /* Cambiando el tamaño del texto */
    settextstyle(0, 0, 3); /* Multiplicador de tamaño x3 */
    setcolor(YELLOW);
    outtextxy(50, 100, "Texto Gigante!");

    /* Alineacion de texto (Centro) */
    settextstyle(0, 0, 2);
    settextjustify(CENTER_TEXT, CENTER_TEXT);
    setcolor(LIGHTGREEN);
    outtextxy(400, 300, "Texto perfectamente centrado en la pantalla");

    /* Midiendo texto */
    char mensaje[] = "Midiendo mi ancho";
    settextjustify(LEFT_TEXT, TOP_TEXT);
    setcolor(LIGHTCYAN);
    outtextxy(50, 450, mensaje);
    
    int ancho = textwidth(mensaje);
    int alto = textheight(mensaje);
    
    /* Dibujar una caja alrededor del texto usando sus medidas */
    rectangle(50, 450, 50 + ancho, 450 + alto);

    getch();
    closegraph();
    return 0;
}
