#include <libgraph3.h>

int main() {
    int gd = DETECT, gm = 0;
    initgraph(&gd, &gm, "");

    setbkcolor(DARKGRAY);
    cleardevice();

    /* Calculamos el centro exacto de la pantalla usando los medidores */
    int centro_x = getmaxx() / 2;
    int centro_y = getmaxy() / 2;

    /* Dibujamos una cruz que cruce toda la pantalla para comprobar los límites */
    setcolor(LIGHTBLUE);
    line(0, centro_y, getmaxx(), centro_y);
    line(centro_x, 0, centro_x, getmaxy());

    /* Preparamos el texto que queremos centrar */
    const char* mensaje = "Este texto esta perfectamente centrado";
    
    /* Calculamos las coordenadas restando la mitad de lo que mide el texto */
    int texto_x = centro_x - (textwidth(mensaje) / 2);
    int texto_y = centro_y - (textheight(mensaje) / 2);

    /* Dibujamos un rectángulo amarillo alrededor del texto */
    setcolor(YELLOW);
    rectangle(texto_x - 10, texto_y - 10, 
              texto_x + textwidth(mensaje) + 10, 
              texto_y + textheight(mensaje) + 10);

    /* Imprimimos el texto en su lugar exacto */
    setcolor(WHITE);
    outtextxy(texto_x, texto_y, mensaje);

    getch(); 
    closegraph();
    
    return 0;
}