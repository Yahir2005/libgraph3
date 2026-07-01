#include <libgraph3.h>
#include <stdio.h>

int main() {
    int gd = DETECT, gm = 0;
    initgraph(&gd, &gm, "");
    setbkcolor(LIGHTGRAY);

    /* Variables de estado para los Text Fields */
    char usuario[50] = "";
    int foco_usuario = 0;

    char clave[50] = "";
    int foco_clave = 0;

    while (!kbhit() || foco_usuario || foco_clave) {
        cleardevice();

        setcolor(BLACK);
        settextjustify(CENTER_TEXT, TOP_TEXT);
        outtextxy(getmaxx() / 2, 50, "Portal de Acceso");

        /* Etiquetas */
        settextjustify(LEFT_TEXT, CENTER_TEXT);
        outtextxy(150, 135, "Usuario:");
        outtextxy(150, 185, "PIN:");

        /* Renderizamos los Text Fields 
           Pasamos el buffer, el límite máximo de caracteres y la variable de foco */
        textfield(250, 120, 200, usuario, 20, &foco_usuario);
        textfield(250, 170, 200, clave, 20, &foco_clave);

        /* Botón de validación que lee lo escrito en los buffers */
        if (button(250, 230, 450, 270, "ENTRAR")) {
            printf("Intentando login con: %s / %s\n", usuario, clave);
        }

        delay(16); // ~60 FPS
        
        if (ismouseclick()) clearmouseclick(); 
    }

    closegraph();
    return 0;
}