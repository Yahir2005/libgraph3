#include <libgraph3.h>

int main() {
    int gd = DETECT, gm = 0;
    initgraph(&gd, &gm, "");

    setbkcolor(BLUE);

    int progreso = 0;
    int cargando = 1;

    /* --- PANTALLA DE CARGA --- */
    while (!kbhit() && cargando) {
        cleardevice();

        setcolor(WHITE);
        settextjustify(CENTER_TEXT, BOTTOM_TEXT);
        outtextxy(getmaxx() / 2, 200, "Iniciando sistema. Por favor espere...");

        progressbar(120, 220, 520, 250, progreso);

        progreso += 1;
        
        if (progreso > 100) {
            /* Hacemos que se detenga medio segundo para que el usuario VEA el 100% */
            delay(500); 
            cargando = 0; 
        }

        delay(40);
        if (ismouseclick()) clearmouseclick();
    }

    /* Limpiamos la basura del teclado por si el usuario presionó algo mientras cargaba */
    if (kbhit()) getch(); 

    /* --- PANTALLA FINAL (Corregida contra el parpadeo) --- */
    /* Todo el dibujo va DENTRO del bucle para que actualice ambos búferes de la GPU */
    while(!kbhit()) { 
        cleardevice();
        
        setcolor(LIGHTGREEN);
        settextjustify(CENTER_TEXT, CENTER_TEXT);
        outtextxy(getmaxx() / 2, getmaxy() / 2, "¡Carga Completada!");
        outtextxy(getmaxx() / 2, getmaxy() / 2 + 40, "Presiona una tecla para salir...");
        
        delay(30); 
    }
    
    closegraph();
    return 0;
}