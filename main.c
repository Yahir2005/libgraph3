#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

/* Detectamos la plataforma base en tiempo de compilación */
#ifdef __APPLE__
    #define IS_MACOS 1
#else
    #define IS_MACOS 0
#endif

int main() {
    int sistema = 0; // 1: Debian/Ubuntu, 2: Fedora, 3: macOS
    char opcion;

    printf("====================================================\n");
    printf("   Instalador Inteligente y Compilador de libgraph3 \n");
    printf("====================================================\n\n");

    /* 1. FASE DE DETECCIÓN DE ENTORNO Y GESTOR DE PAQUETES */
    if (IS_MACOS) {
        sistema = 3;
        printf("[INFO] Plataforma detectada: macOS\n");
        printf("[INFO] Gestor de paquetes objetivo: Homebrew (brew)\n");
    } else {
        /* Si estamos en Linux, buscamos qué gestor de paquetes responde */
        if (system("which apt > /dev/null 2>&1") == 0) {
            sistema = 1;
            printf("[INFO] Plataforma detectada: Distribución basada en Debian/Ubuntu\n");
            printf("[INFO] Gestor de paquetes objetivo: Advanced Package Tool (apt)\n");
        } else if (system("which dnf > /dev/null 2>&1") == 0) {
            sistema = 2;
            printf("[INFO] Plataforma detectada: Distribución basada en Fedora/RHEL\n");
            printf("[INFO] Gestor de paquetes objetivo: Dandified YUM (dnf)\n");
        } else {
            printf("[ERROR] No se encontró un gestor compatible (apt, dnf o brew) en el PATH.\n");
            return 1;
        }
    }

    /* 2. PREPARACIÓN Y GESTIÓN DE DEPENDENCIAS DEL SISTEMA */
    printf("\n¿Deseas validar e instalar las herramientas de compilación del sistema (cmake, git, gcc)? [s/n]: ");
    scanf(" %c", &opcion);

    if (opcion == 's' || opcion == 'S') {
        printf("\n[1/4] Instalando pre-requisitos de desarrollo...\n");
        printf("----------------------------------------------------\n");
        if (sistema == 1) {
            system("sudo apt update && sudo apt install -y cmake git build-essential");
        } else if (sistema == 2) {
            system("sudo dnf check-update; sudo dnf groupinstall -y \"Development Tools\" && sudo dnf install -y cmake git");
        } else if (sistema == 3) {
            system("brew install cmake git");
        }
        printf("----------------------------------------------------\n");
    } else {
        printf("\n[1/4] Saltando validación de dependencias del sistema.\n");
    }

    /* 3. COMPILACIÓN ATÓMICA CON CMAKE Y MAKE */
    printf("\n[2/4] Lanzando entorno de construcción interna...\n");
    printf("[CMake] Descargando y compilando sub-módulos (FETCHCONTENT_QUIET=OFF)...\n");
    printf("----------------------------------------------------\n");
    
    /* El script crea la carpeta, entra en ella, arranca el parseo visual y compila */
    int build_status = system("mkdir -p build && cd build && cmake .. -DFETCHCONTENT_QUIET=OFF && make");
    printf("----------------------------------------------------\n");

    if (build_status != 0) {
        printf("\n[ERROR] El proceso de compilación nativa falló (Código: %d).\n", build_status);
        printf("Verifica los mensajes de error del compilador impresos arriba.\n");
        return 1;
    }
    printf("[OK] Compilación finalizada exitosamente.\n");

    /* 4. ENLAZADO DINÁMICO E INSTALACIÓN SEGÚN ARQUITECTURA */
    printf("\n[3/4] Desplegando binarios en las rutas compartidas del sistema...\n");
    
    /* Mapeamos la extensión correcta para evitar fallos de linkeo (.so en Linux, .dylib en Mac) */
    const char *ext = (sistema == 3) ? "dylib" : "so";
    char comando[512];

    /* Copiamos los binarios intermedios de SDL3 */
    sprintf(comando, "sudo cp -RP $(find build/_deps -name 'libSDL3.%s*') /usr/local/lib/ 2>/dev/null", ext);
    system(comando);

    /* Copiamos los binarios de fuentes de SDL3_ttf */
    sprintf(comando, "sudo cp -RP $(find build/_deps -name 'libSDL3_ttf.%s*') /usr/local/lib/ 2>/dev/null", ext);
    system(comando);

    /* Copiamos el archivo compilado final de libgraph3 */
    sprintf(comando, "sudo cp -RP build/libgraph3.%s /usr/local/lib/ 2>/dev/null", ext);
    system(comando);

    /* Copiamos la cabecera global pública */
    system("sudo cp libgraph3.h /usr/local/include/ 2>/dev/null");

    /* 5. REFRESCAR LA CACHÉ DEL ENLAZADOR DINÁMICO */
    printf("\n[4/4] Finalizando configuraciones locales...\n");
    if (sistema != 3) {
        /* ldconfig es exclusivo de sistemas Linux */
        system("sudo ldconfig");
        printf("[OK] Caché del sistema actualizada mediante ldconfig.\n");
    } else {
        printf("[OK] Entorno macOS configurado (salto de ldconfig estructural).\n");
    }

    printf("\n====================================================\n");
    printf(" ¡Proceso completado con éxito global!\n");
    printf(" El motor ya está disponible de manera universal en tu equipo.\n");
    printf(" Compila tus códigos fuente desde cualquier directorio con:\n");
    printf(" gcc tu_codigo.c -o ejecutable -lgraph3 -lSDL3 -lSDL3_ttf -lm\n");
    printf("====================================================\n");

    return 0;
}