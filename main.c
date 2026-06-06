#include <libgraph3.h> // ¡Nota que ahora usamos <> como las librerías oficiales!

int main() {
    int gd = 0, gm = 0;
    initgraph(&gd, &gm, "");
    
    setbkcolor(BLUE);
    cleardevice();
    setcolor(WHITE);
    outtextxy(100, 100, "¡Compilando desde cualquier carpeta!");
    
    getch();
    closegraph();
    return 0;
}