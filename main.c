#include <libgraph3.h>
#include <stdio.h>

int main() {
    int gd = DETECT, gm = 0;
    initgraph(&gd, &gm, "");

    setbkcolor(DARKGRAY);

    /* La variable maestra (El "estado" en arquitectura de UI) */
    int volumen_actual = 50; 
    char texto_estado[50];

    while (!kbhit()) {
        cleardevice();

        setcolor(YELLOW);
        settextjustify(CENTER_TEXT, TOP_TEXT);
        outtextxy(getmaxx() / 2, 50, "Controles de Audio");

        /* 1. Dibujamos el Slider, le pasamos la longitud (200px) y la variable */
        slider(200, 150, 200, "Volumen Maestro", &volumen_actual);

        /* 2. Visualizamos el cambio en tiempo real con la barra que hicimos antes */
        progressbar(200, 250, 400, 280, volumen_actual);

        /* 3. Mostrar el valor exacto para debuggear */
        sprintf(texto_estado, "Nivel en memoria: %d", volumen_actual);
        setcolor(WHITE);
        outtextxy(getmaxx() / 2, 320, texto_estado);

        delay(16); 
        if (ismouseclick()) clearmouseclick(); 
    }

    closegraph();
    return 0;
}