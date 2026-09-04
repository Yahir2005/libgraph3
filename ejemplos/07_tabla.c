#include <libgraph3.h>
#include <stdio.h>

int main() {
    initwindow(800, 600, "07 - Tabla interactiva (Estilo JTable)");

    const char *headers[] = {"ID", "Nombre Alumno", "Calificacion", "Estado"};
    
    /* Matriz plana (1D) para los datos: 5 filas x 4 columnas */
    const char *data[] = {
        "101", "Ana Martinez",   "95", "Aprobado",
        "102", "Luis Gomez",     "73", "Aprobado",
        "103", "Carlos Ruiz",    "58", "Reprobado",
        "104", "Maria Lopez",    "88", "Aprobado",
        "105", "Jorge Perez",    "42", "Reprobado"
    };

    int selected_row = -1;
    int selected_col = -1;
    
    char mensaje_seleccion[100] = "Ningun alumno seleccionado.";

    while (1) {
        if (kbhit() && getch() == 27) break; /* Presiona ESC para salir */

        cleardevice();
        
        setcolor(WHITE);
        outtextxy(10, 10, "Componente de Tabla (Haz clic en un alumno para seleccionarlo. ESC para salir)");

        /* Dibujar la tabla interactiva */
        if (drawtable(50, 50, 5, 4, 150, 40, headers, data, &selected_row, &selected_col)) {
            /* Si drawtable devuelve 1, significa que el usuario acaba de hacer clic */
            if (selected_row >= 0 && selected_row < 5) {
                sprintf(mensaje_seleccion, "Seleccionaste a: %s (ID: %s) - %s", 
                        data[selected_row * 4 + 1],  /* Columna 1 (Nombre) */
                        data[selected_row * 4 + 0],  /* Columna 0 (ID) */
                        data[selected_row * 4 + 3]); /* Columna 3 (Estado) */
            }
        }

        /* Mostrar el detalle de la selección debajo de la tabla */
        setcolor(YELLOW);
        outtextxy(50, 350, mensaje_seleccion);

        delay(16);
    }

    closegraph();
    return 0;
}
