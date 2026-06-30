#include "libgraph3.h"
#include <stdio.h>

int main() {
    int gd = DETECT, gm = 0;
    initgraph(&gd, &gm, "");

    setbkcolor(BLUE);
    cleardevice();

    /* 1. Ponemos algunas cosas de fondo para comprobar que no se borran */
    setcolor(YELLOW);
    circle(100, 100, 50);
    setfillstyle(SOLID_FILL, LIGHTGREEN);
    bar(400, 300, 600, 400);

    /* 2. Llamamos a tu equivalente de JOptionPane (Frena el programa aquí) */
    int edad = inputdialog_int("Ingrese su edad actual (solo numeros):");

    /* 3. El cuadro ya desapareció. Hacemos algo con el Integer resultante */
    int ano_nacimiento = 2026 - edad;

    /* 4. Mostramos el resultado */
    char mensaje_final[100];
    sprintf(mensaje_final, "Tu naciste aproximadamente en el ano %d", ano_nacimiento);
    
    settextstyle(0, 0, 2);
    settextjustify(CENTER_TEXT, CENTER_TEXT);
    setcolor(WHITE);
    outtextxy(getmaxx() / 2, getmaxy() / 2, mensaje_final);
    
    outtextxy(getmaxx() / 2, getmaxy() - 50, "Presiona una tecla para salir...");

    getch(); 
    closegraph();
    
    return 0;
}