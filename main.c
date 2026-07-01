#include <libgraph3.h>
#include <stdio.h>   // para printf

int main() {
    int gd = DETECT, gm = 0;
    initgraph(&gd, &gm, "");

    int aceptar_terminos = 0;
    int recibir_correos = 1;   // marcado por defecto

    setbkcolor(DARKGRAY);

    while (!kbhit()) {
        cleardevice();

        setcolor(YELLOW);
        outtextxy(50, 50, "Formulario de Registro");

        // Dibujamos los checkboxes
        checkbox(50, 100, "Acepto los terminos y condiciones", &aceptar_terminos);
        checkbox(50, 140, "Deseo recibir correos con ofertas", &recibir_correos);

        // Mostramos el estado actual de las variables en la pantalla (depuración)
        char buffer[100];
        sprintf(buffer, "Aceptar: %d  |  Correos: %d", aceptar_terminos, recibir_correos);
        setcolor(WHITE);
        outtextxy(50, 300, buffer);

        if (aceptar_terminos) {
            if (button(50, 200, 150, 240, "REGISTRAR")) {
                // Aquí podrías poner un mensaje o acción
                setcolor(GREEN);
                outtextxy(50, 270, "¡Registro exitoso! (simulado)");
            }
        } else {
            setcolor(LIGHTRED);
            outtextxy(50, 210, "Debes aceptar los terminos para continuar.");
        }

        delay(60);

        // Limpiamos clics no consumidos (importante)
        if (ismouseclick()) {
            clearmouseclick();
        }
    }

    closegraph();
    return 0;
}