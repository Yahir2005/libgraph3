/* ============================================================
 *  EJEMPLO HÍBRIDO:  C + asm{}  +  libgraph3
 *
 *  Compilar:
 *      cmake -B build && cmake --build build
 *
 *  El preprocesador asm2c.py convierte cada bloque asm{...}
 *  en llamadas int86() automáticamente.
 *
 *  INSTRUCCIONES ASM SOPORTADAS:
 *    mov, int, add, sub, xor, and, or, inc, dec, not, neg
 * ============================================================ */

#include "libgraph3.h"   /* API gráfica BGI sobre SDL3         */
#include "dos.h"         /* int86(), union REGS, pseudo-regs   */
#include <stdio.h>
#include <stdlib.h>

int main() {
    int gd = DETECT, gm = 0;
    char texto[64];

    /* ------------------------------------------------------ */
    /*  1. INICIALIZAR MODO GRÁFICO  (con asm{})             */
    /*     Equivalente a:  int 10h, ah=0, al=0x13            */
    /*     (320x200, 256 colores — simulado en ventana SDL)  */
    /* ------------------------------------------------------ */
    initgraph(&gd, &gm, "");

    /* ------------------------------------------------------ */
    /*  2. LEER FECHA DEL SISTEMA  (vía interrupción DOS)    */
    /*     Equivalente a:  mov ah,2Ah  int 21h               */
    /*     Devuelve:  cx=año, dh=mes, dl=día, al=día_semana */
    /* ------------------------------------------------------ */
    asm{mov ah,2Ah,int 21h}

    /* Después del asm{}, los pseudo-registros _CX, _DH, _DL,
     * _AL contienen los valores devueltos por la interrupt.
     * También podemos usar una union REGS manualmente:      */
    {
        union REGS r;
        r.h.ah = 0x2A;
        int86(0x21, &r, &r);
        sprintf(texto, "Fecha via int86:  %02u/%02u/%04u",
                (unsigned)r.h.dl, (unsigned)r.h.dh, (unsigned)r.x.cx);
    }
    setcolor(LIGHTGREEN);
    outtextxy(10, 10, texto);

    /* ------------------------------------------------------ */
    /*  3. LEER HORA DEL SISTEMA  (asm{} multilínea)         */
    /*     Equivalente a:  mov ah,2Ch  int 21h               */
    /*     Devuelve:  ch=hora, cl=min, dh=seg                */
    /* ------------------------------------------------------ */
    asm {
        mov ah, 2Ch
        int 21h
    }

    sprintf(texto, "Hora via asm{}:   %02u:%02u:%02u",
            (unsigned)_CH, (unsigned)_CL, (unsigned)_DH);
    setcolor(LIGHTCYAN);
    outtextxy(10, 30, texto);

    /* ------------------------------------------------------ */
    /*  4. ARITMÉTICA EN ASM  (para mostrar registros)       */
    /*     Cargamos valores, los sumamos y restamos.         */
    /* ------------------------------------------------------ */
    asm{mov ax,000Ah,mov bx,0005h,add ax,bx,sub bx,0002h}

    sprintf(texto, "Asm aritmetica:  ax=%04X  bx=%04X",
            (unsigned)_AX, (unsigned)_BX);
    setcolor(YELLOW);
    outtextxy(10, 50, texto);

    /* Usamos los mismos valores desde la union REGS          */
    {
        union REGS r;
        r.x.ax = 0x000A;
        r.x.bx = 0x0005;
        asm{add ax,bx,sub bx,0002h}
        /* Tras el asm{}, r ya no refleja los cambios porque
         * el asm{} usa su propia variable __r interna.
         * Para leer resultados, usamos _AX, _BX globales.   */
        sprintf(texto, "Asm aritmetica 2:  ax=%04X  bx=%04X",
                (unsigned)_AX, (unsigned)_BX);
    }
    setcolor(LIGHTRED);
    outtextxy(10, 70, texto);

    /* ------------------------------------------------------ */
    /*  5. DIBUJAR CON libgraph3  (puro C, sin asm)          */
    /* ------------------------------------------------------ */
    setbkcolor(BLACK);
    cleardevice();

    setfillstyle(SOLID_FILL, LIGHTBLUE);
    bar(10, 10, 40, 40);
    setcolor(YELLOW);
    circle(25, 25, 10);
    line(10, 10, 40, 40);
    line(40, 10, 10, 40);

    /* Capturamos el sprite a memoria                         */
    unsigned int tam = imagesize(0, 0, 50, 50);
    void *sprite = malloc(tam);
    getimage(0, 0, 50, 50, sprite);

    /* Fondo decorativo                                       */
    cleardevice();
    setcolor(DARKGRAY);
    for (int i = 0; i < 80; i++)
        circle(rand() % getmaxx(), rand() % getmaxy(), rand() % 15 + 3);

    /* Mostramos instrucciones                               */
    setcolor(WHITE);
    outtextxy(10, getmaxy() - 50,
        "Demo hibrido C + asm{}  —  Presiona ESC o Q para salir");

    /* ------------------------------------------------------ */
    /*  6. BUCLE PRINCIPAL  con kbhit via asm{}              */
    /*     Equivalente a:  mov ah,01h  int 16h               */
    /*     (ZF=0  → hay tecla;  ZF=1 → no hay tecla)        */
    /* ------------------------------------------------------ */
    int x = 50, y = 50;
    int dx = 4, dy = 3;

    while (1) {
        putimage(x, y, sprite, COPY_PUT);
        delay(16);

        /* Verificamos tecla vía asm{} (int 16h, ah=01h)    */
        asm {
            mov ah, 01h
            int 16h
        }

        /* Si hay tecla (ZF=0 ~flags bit 6), la leemos      */
        {
            union REGS chk;
            chk.h.ah = 0x01;
            int86(0x16, &chk, &chk);
            if (!(chk.x.flags & 0x40)) {
                /* Leer la tecla (int 16h, ah=00h)           */
                asm {
                    mov ah, 00h
                    int 16h
                }
                /* _AL contiene el ASCII; _AH el scan code  */
                if (_AL == 0x1B || _AL == 'q' || _AL == 'Q')
                    break;
            }
        }

        x += dx;
        y += dy;
        if (x < 0 || x > getmaxx() - 50) dx = -dx;
        if (y < 0 || y > getmaxy() - 50) dy = -dy;
    }

    /* ------------------------------------------------------ */
    /*  7. SALIR  mostrando mensaje con asm{}                */
    /*     Equivalente a:  mov ah,09h  int 21h               */
    /*     (imprime string terminado en '$')                 */
    /* ------------------------------------------------------ */
    /* No podemos pasar un string C directamente a int 21h/09
     * porque requiere un puntero DOS en ds:dx.
     * Usamos printf de C normal:                            */
    setcolor(LIGHTGREEN);
    outtextxy(getmaxx() / 2 - 60, getmaxy() / 2,
              "Fin del demo hibrido C + asm{}");

    free(sprite);
    closegraph();
    return 0;
}
