# libgraph3: BGI / `<graphics.h>` Moderno para Linux

**libgraph3** es una reimplementación moderna, ligera y acelerada por hardware de la clásica librería `graphics.h` (Borland Graphics Interface - BGI) de MS-DOS. Está construida en C puro utilizando **SDL 3** y **CMake**, diseñada específicamente para compilar y funcionar de manera nativa en distribuciones Linux modernas sin las dependencias obsoletas de hace 20 años.

## 🚀 Motivación

La librería original `libgraph` (1.0.2) lleva abandonada desde 2006. Sus dependencias en SDL 1.2 y Guile, sumadas a su antiguo sistema de construcción *Autotools*, hacen que sea casi imposible instalarla en sistemas actuales sin múltiples errores de compilación (`-fcommon`, *undefined references*, etc.).

Este proyecto nace para ayudar a estudiantes de ingeniería de software y entusiastas de la programación a modernizar sus algoritmos y tareas clásicas. **libgraph3** reemplaza la emulación de software en RAM por aceleración gráfica directa en VRAM (GPU), manteniendo intacta la simplicidad de uso de la API original de Borland.

## ✨ Características Principales

* **100% Acelerada por Hardware (Anti-Flicker):** Las primitivas geométricas envían instrucciones directas a la tarjeta gráfica mediante SDL 3. Utiliza una textura persistente como memoria de video para garantizar **cero parpadeos** (*flickering*) en animaciones.
* **True Color y Retrocompatibilidad:** Utiliza la nueva macro `COLOR(r,g,b)` para renderizar millones de colores, mientras mantiene la compatibilidad 100% estricta con los clásicos colores DOS (`YELLOW`, `RED`, etc.) de Turbo C.
* **Resoluciones HD:** Soporte para resoluciones dinámicas mediante `initwindow(width, height)`.
* **Carga de Imágenes (Sprites):** Soporte para capturar y pegar porciones de la VRAM (`getimage` / `putimage`), y para cargar archivos gráficos externos (`readimagefile`) ideal para videojuegos retro.
* **Teclado Asíncrono:** Funciones como `kbhit()` permiten leer el teclado en tiempo real sin bloquear la ejecución del programa.
* **Tipografía Dinámica e Independiente:** Motor de texto TrueType (`SDL3_ttf`) con escalado al vuelo. Descarga automáticamente una fuente libre durante la instalación, asegurando total portabilidad.
* **Interfaz Gráfica Integrada:** Soporte para widgets como `button`, `slider`, `progressbar`, `checkbox`, y detección fácil del ratón.

---

## 🛠️ Instalación Rápida (Un Clic)

Todo el proceso de descarga de dependencias, compilación, e instalación en el sistema operativo ha sido automatizado en un único *script* inteligente. Soporta **Debian/Ubuntu, Fedora y macOS**.

Abre tu terminal y ejecuta:

```bash
# 1. Clonar el repositorio
git clone https://github.com/<TU_USUARIO>/libgraph3.git
cd libgraph3

# 2. Ejecutar el Instalador
chmod +x install.sh
./install.sh
```

El instalador te preguntará si deseas bajar las dependencias (como `cmake`, `git` o herramientas X11/Wayland), descargará las fuentes necesarias, compilará la librería, instalará los binarios en tu sistema e inyectará el atajo universal de compilación.

---

## 💻 Guía de Uso

Ya no tienes que escribir pesadas instrucciones `gcc` incluyendo el motor de SDL.
Gracias a que `install.sh` registra un comando global en el sistema, para compilar cualquier código puedes usar simplemente:

```bash
cgraph mi_programa.c
```
*Esto autocompletará el enlace y te entregará inmediatamente tu archivo ejecutable `./mi_programa`.*

### Directorio de Ejemplos
Dentro del repositorio clonado encontrarás una carpeta llamada `ejemplos/`. Te sugerimos explorar y compilar los archivos (`01_basico.c`, `02_colores.c`, ..., `06_avanzado.c`) para dominar rápidamente la librería.

### Código Básico (Hola Mundo Gráfico)

```c
#include <libgraph3.h>

int main() {
    // 1. Inicializar ventana de 800x600
    initwindow(800, 600, "Mi Primer Juego");

    // 2. Colores y Dibujo
    setbkcolor(DARKGRAY);
    cleardevice();

    setcolor(COLOR(0, 255, 128)); // True Color
    circle(400, 300, 100);
    
    setfillstyle(SOLID_FILL, BLUE); // Clásico BGI
    floodfill(400, 300, COLOR(0, 255, 128));

    // 3. Interacción
    setcolor(WHITE);
    outtextxy(10, 10, "Presiona una tecla...");
    getch(); 

    closegraph();
    return 0;
}
```

---

## 📚 Referencia de la API (Funciones Portadas)

**Control del Sistema:**
* `initgraph(int *gd, int *gm, const char *path)` *(Modo Legacy)*
* `initwindow(int width, int height, const char* title)` *(Modo Moderno)*
* `closegraph()`
* `delay(int millis)`

**Lectura de Entorno y Estado:**
* `getmaxx()`, `getmaxy()`
* `getx()`, `gety()`
* `getcolor()`, `getbkcolor()`
* `textwidth(const char *text)`, `textheight(const char *text)`

**Interacción (Teclado y Ratón):**
* `kbhit()`: Verifica si hay teclas en cola sin pausar el hilo.
* `getch()`: Captura la tecla presionada.
* `mousex()`, `mousey()`, `ismouseclick()`, `clearmouseclick()`, `ismousedown()`

**Geometría y Relleno:**
* `putpixel(int x, int y, int color)`
* `line(int x1, int y1, int x2, int y2)`
* `rectangle(int left, int top, int right, int bottom)`
* `circle(int x, int y, int radius)`
* `ellipse(int x, int y, int stangle, int endangle, int xradius, int yradius)`
* `arc(int x, int y, int stangle, int endangle, int radius)`
* `drawpoly(int numpoints, int *polypoints)`
* `bar(int left, int top, int right, int bottom)`
* `floodfill(int x, int y, int border)`

**Colores y Estilos:**
* `setcolor(int color)`
* `setbkcolor(int color)`
* `setfillstyle(int pattern, int color)`
* `cleardevice()`
* Macro `COLOR(R, G, B)`

**Texto:**
* `outtextxy(int x, int y, const char *text)`
* `settextstyle(int font, int direction, int charsize)`
* `settextjustify(int horiz, int vert)`

**Memoria Visual e Imágenes (Sprites):**
* `imagesize(int left, int top, int right, int bottom)`
* `getimage(int left, int top, int right, int bottom, void *bitmap)`
* `putimage(int left, int top, void *bitmap, int op)`
* `readimagefile(const char *filename, int left, int top, int right, int bottom)`

**Controles de Interfaz de Usuario (UI):**
* `button(left, top, right, bottom, text)`
* `checkbox(x, y, text, &estado)`
* `slider(x, y, len, text, &valor)`
* `progressbar(left, top, right, bottom, porcentaje)`
* `inputdialog_int(prompt)`
* `drawtable(x, y, rows, cols, w, h, headers, data, &sel_row, &sel_col)`
