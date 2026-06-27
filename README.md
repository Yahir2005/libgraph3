# libgraph3: BGI / `<graphics.h>` Moderno para Linux

**libgraph3** es una reimplementación moderna, ligera y acelerada por hardware de la clásica librería `graphics.h` (Borland Graphics Interface - BGI) de MS-DOS. Está construida en C puro utilizando **SDL 3** y **CMake**, diseñada específicamente para compilar y funcionar de manera nativa en distribuciones Linux modernas sin las dependencias obsoletas de hace 20 años.

## 🚀 Motivación

La librería original `libgraph` (1.0.2) lleva abandonada desde 2006. Sus dependencias en SDL 1.2 y Guile, sumadas a su antiguo sistema de construcción *Autotools*, hacen que sea casi imposible instalarla en sistemas actuales sin múltiples errores de compilación (`-fcommon`, *undefined references*, etc.).

Este proyecto nace para ayudar a estudiantes de ingeniería de software y entusiastas de la programación a modernizar sus algoritmos y tareas clásicas. **libgraph3** reemplaza la emulación de software en RAM por aceleración gráfica directa en VRAM (GPU), manteniendo intacta la simplicidad de uso de la API original de Borland.

## ✨ Características Principales

* **100% Acelerada por Hardware:** Las primitivas geométricas envían instrucciones directas a la tarjeta gráfica mediante SDL 3.
* **Geometría y Relleno Avanzados:** Soporte para polígonos, trigonometría (elipses/arcos) y un motor de relleno (`floodfill`) que cruza eficientemente la GPU y la CPU usando DFS iterativo.
* **Memoria Visual (Sprites):** Soporte nativo para capturar y pegar porciones de la VRAM (`getimage` / `putimage`) a 60 FPS, ideal para videojuegos retro.
* **Teclado Asíncrono:** Funciones como `kbhit()` permiten leer el teclado en tiempo real sin bloquear la ejecución del programa.
* **Tipografía Dinámica:** Motor de texto TrueType (`SDL3_ttf`) con escalado al vuelo, justificación matemática y anti-aliasing.
* **Bilingüe (C y C++):** Compatible con `gcc` y `g++` gracias a su integración `extern "C"`.

---

## 🛠️ Instalación

### 1. Dependencias Previas

En distribuciones basadas en Debian/Ubuntu, instala las herramientas de compilación y dependencias de ventanas/fuentes:

```bash
sudo apt update
sudo apt install build-essential cmake git libfreetype-dev libharfbuzz-dev libx11-dev libwayland-dev libxkbcommon-dev libxext-dev libxrender-dev libxcursor-dev libxi-dev libxfixes-dev libxrandr-dev libxss-dev libxtst-dev

```

*(Nota: CMake descargará automáticamente el núcleo de SDL 3 desde GitHub durante la compilación).*

### 2. Clonar y Compilar

Abre tu terminal y ejecuta los siguientes comandos para instalar la librería de forma universal en tu sistema:

```bash
# 1. Clonar el repositorio
git clone https://github.com/<TU_USUARIO>/libgraph3.git
cd libgraph3

# 2. Crear entorno de compilación
mkdir build && cd build
cmake ..

# 3. Compilar la librería compartida (.so)
make

# 4. Instalar en /usr/local/lib y /usr/local/include
sudo make install

```

### 3. Configuración Post-Instalación

Para que el sistema operativo registre correctamente los enlaces dinámicos generados por CMake, ejecuta:

```bash
# Copiar las dependencias de SDL3 respetando enlaces simbólicos
sudo find . -name "libSDL3*.so*" -exec cp -P {} /usr/local/lib/ \;

# Actualizar la caché del enlazador de Linux
sudo ldconfig

```

---

## 💻 Guía de Uso

Puedes incluir `<libgraph3.h>` en cualquier directorio de tu computadora. Para compilar tu código, simplemente añade la bandera `-lgraph3` y `-lm` (para las funciones matemáticas de C).

```bash
gcc mi_juego.c -o juego -lgraph3 -lm
./juego

```

### Ejemplo: Interfaz Dinámica y Floodfill

```c
#include <libgraph3.h>

int main() {
    int gd = DETECT, gm = 0;
    initgraph(&gd, &gm, "");

    setbkcolor(LIGHTGRAY);
    cleardevice();

    // 1. Uso de Medidores de Entorno
    int cx = getmaxx() / 2;
    int cy = getmaxy() / 2;

    // 2. Geometría y Bote de Pintura (Floodfill)
    setcolor(YELLOW);
    circle(cx - 40, cy, 60);
    circle(cx + 40, cy, 60);

    setfillstyle(SOLID_FILL, LIGHTBLUE);
    floodfill(cx, cy, YELLOW); // Rellena la intersección de los círculos

    // 3. Texto Dinámico Justificado
    settextstyle(0, 0, 2); // Tamaño x2
    settextjustify(CENTER_TEXT, BOTTOM_TEXT);
    setcolor(BLACK);
    outtextxy(cx, cy - 70, "GRAFICOS ACELERADOS");

    // 4. Interacción en Tiempo Real
    while (!kbhit()) {
        // El programa corre libremente hasta presionar una tecla
        delay(16);
    }

    getch(); // Limpia el buffer
    closegraph();
    
    return 0;
}

```

---

## 📚 Referencia de la API (Funciones Portadas)

**Control del Sistema:**

* `initgraph(int *gd, int *gm, const char *path)`
* `closegraph()`
* `delay(int millis)`

**Lectura de Entorno y Estado (Getters):**

* `getmaxx()`, `getmaxy()`
* `getx()`, `gety()`
* `getcolor()`, `getbkcolor()`
* `textwidth(const char *text)`, `textheight(const char *text)`

**Interacción (Teclado):**

* `kbhit()`: Verifica si hay teclas en cola sin pausar el hilo.
* `getch()`: Captura la tecla presionada.

**Cursor Gráfico (Current Pointer):**

* `moveto(int x, int y)`
* `lineto(int x, int y)`
* `moverel(int dx, int dy)`
* `linerel(int dx, int dy)`

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

**Texto:**

* `outtextxy(int x, int y, const char *text)`
* `settextstyle(int font, int direction, int charsize)`
* `settextjustify(int horiz, int vert)`

**Memoria Visual (Sprites):**

* `imagesize(int left, int top, int right, int bottom)`
* `getimage(int left, int top, int right, int bottom, void *bitmap)`
* `putimage(int left, int top, void *bitmap, int op)`