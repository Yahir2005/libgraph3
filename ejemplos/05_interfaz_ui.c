#include <libgraph3.h>
#include <stdio.h>

int main() {
    initwindow(800, 600, "05 - Controles UI (Exclusivo de libgraph3)");

    int checkbox_activo = 0;
    int slider_valor = 50;

    while (1) {
        if (kbhit() && getch() == 27) break; /* ESC para salir */

        cleardevice();
        
        setcolor(WHITE);
        outtextxy(10, 10, "Controles de Interfaz (Exclusivos de este motor). ESC para salir.");

        /* 1. Boton */
        if (button(50, 100, 200, 150, "Boton de Prueba")) {
            /* Hacer algo cuando se presiona */
            slider_valor = 0;
        }

        /* 2. Checkbox */
        checkbox(50, 200, "Activar opcion especial", &checkbox_activo);

        /* 3. Slider (Barra deslizante) */
        slider(50, 300, 200, "Volumen", &slider_valor);

        /* 4. Barra de progreso */
        progressbar(50, 400, 300, 430, slider_valor);

        /* Elemento condicional basado en el checkbox */
        if (checkbox_activo) {
            setcolor(LIGHTGREEN);
            outtextxy(300, 200, "-> Opcion activada!");
        }

        /* 5. Cuadro de dialogo de entrada (Se activa con clic derecho o algo, aqui lo haremos con un boton extra) */
        if (button(50, 500, 250, 550, "Abrir Input Dialog")) {
            slider_valor = inputdialog_int("Escribe un numero (0-100):");
        }

        delay(16);
    }

    closegraph();
    return 0;
}
