# libgraph2

libgraph3: BGI / <graphics.h> Moderno para Linux

libgraph3 es una reimplementación moderna, ligera y acelerada por hardware de la clásica librería graphics.h (Borland Graphics Interface - BGI) de MS-DOS. Está construida en C puro utilizando SDL 3 y CMake, diseñada específicamente para compilar y funcionar de manera nativa en distribuciones Linux modernas sin las dependencias obsoletas de hace 20 años.

✨ Características Principales
100% Acelerada por Hardware: Usa el backend nativo de SDL 3. Las primitivas geométricas envían instrucciones directas a la tarjeta gráfica.

Bilingüe (C y C++): Compatible con gcc y g++ gracias a su integración extern "C".

Texto Moderno: Adiós al bitmap de 8x8. Soporta tipografías TrueType (.ttf) con anti-aliasing real mediante SDL3_ttf.

Interacción Asíncrona: La función getch() ha sido reescrita para usar la cola de eventos moderna. Tu programa ya no congelará el uso de CPU de tu sistema operativo mientras espera una tecla.

Fácil Instalación: Sistema de compilación unificado de 30 líneas en CMake que descarga automáticamente las dependencias dinámicas.

🛠️ Instalación
1. Dependencias Previas
Antes de compilar, necesitas instalar las herramientas de desarrollo y los motores tipográficos básicos. En distros basadas en Debian/Ubuntu, ejecuta:

sudo apt update
sudo apt install build-essential cmake git libfreetype-dev libharfbuzz-dev

(Nota: No necesitas instalar SDL 3 manualmente, CMake lo descargará de los repositorios oficiales por ti).

2. Clonar y Compilar
Abre tu terminal y ejecuta los siguientes comandos para compilar la librería e instalarla de forma universal en tu sistema:

# 1. Clonar el repositorio
git clone https://github.com/TU_USUARIO/libgraph3.git
cd libgraph3

# 2. Crear entorno de compilación
mkdir build && cd build
cmake ..

# 3. Compilar la librería compartida (.so)
make

# 4. Instalar en /usr/local/lib y /usr/local/include
sudo make install

3. Configuración Post-Instalación
Como CMake descarga las librerías dinámicas de SDL3 al vuelo, necesitamos copiarlas al directorio principal de tu sistema para que el enlazador de Linux las encuentre siempre:

# Copiar las librerías dinámicas base respetando enlaces simbólicos
sudo find . -name "libSDL3*.so*" -exec cp -P {} /usr/local/lib/ \;

# Actualizar la caché del sistema operativo
sudo ldconfig

💻 Guía de Uso
Una vez instalada, puedes usar <libgraph3.h> desde cualquier directorio de tu computadora, tal como lo harías con las librerías estándar.

Ejemplo de código (main.c)
#include <libgraph3.h>
#include <stdio.h>

int main() {
    // 1. Inicializar la ventana
    int gd = DETECT, gm = 0;
    initgraph(&gd, &gm, "");

    // 2. Configurar colores y limpiar pantalla
    setbkcolor(DARKGRAY);
    cleardevice();

    // 3. Dibujar geometría acelerada por GPU
    setcolor(LIGHTBLUE);
    rectangle(200, 250, 440, 450);

    setcolor(LIGHTRED);
    line(200, 250, 320, 150);
    line(320, 150, 440, 250);

    setcolor(WHITE);
    circle(320, 320, 40);

    // 4. Renderizar texto moderno
    setcolor(YELLOW);
    outtextxy(180, 400, "¡Emulando BGI en Linux Moderno!");

    // 5. Esperar interacción sin bloquear el hilo principal
    getch(); 
    
    // 6. Limpiar VRAM y cerrar
    closegraph();
    
    return 0;
}

Cómo compilar tus programas
Usa el compilador estándar (gcc o g++) y añade la bandera -lgraph3 al final:

# Para lenguaje C
gcc main.c -o aplicacion -lgraph3

# Para lenguaje C++
g++ main.cpp -o aplicacion -lgraph3

# Ejecutar
./aplicacion

📚 Funciones Portadas
Hasta el momento, libgraph3 soporta las siguientes llamadas clásicas:

Control del Sistema:

initgraph(int *gd, int *gm, const char *path): Abre la ventana de SDL 3.

closegraph(): Libera recursos y cierra SDL.

delay(int millis): Pausa asíncrona no bloqueante.

getch(): Captura teclas de forma pasiva escuchando eventos del SO.

Color y Pincel:

Soporte completo para la paleta BGI (Macros de BLACK a WHITE).

setcolor(int color): Define color de líneas/texto.

setbkcolor(int color): Define el fondo.

cleardevice(): Limpia la pantalla visualmente al instante.

Dibujo y Geometría:

putpixel(int x, int y, int color)

line(int x1, int y1, int x2, int y2)

rectangle(int left, int top, int right, int bottom)

circle(int x, int y, int radius) (Usa algoritmo de Bresenham optimizado para GPU)

Texto:

outtextxy(int x, int y, const char *text): Renderiza cadenas de caracteres en coordenadas específicas.
