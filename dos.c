#include "dos.h"
#include <SDL3/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

unsigned short _AX, _BX, _CX, _DX, _SI, _DI, _SP, _BP;
unsigned char _AL, _AH, _BL, _BH, _CL, _CH, _DL, _DH;

static int text_cursor_row = 0;
static int text_cursor_col = 0;
static int video_mode = 3;

static void sync_from_regs(union REGS *r)
{
    _AX = r->x.ax; _BX = r->x.bx; _CX = r->x.cx; _DX = r->x.dx;
    _SI = r->x.si; _DI = r->x.di;
    _AL = r->h.al; _AH = r->h.ah; _BL = r->h.bl; _BH = r->h.bh;
    _CL = r->h.cl; _CH = r->h.ch; _DL = r->h.dl; _DH = r->h.dh;
}

static void sync_to_regs(union REGS *r)
{
    r->x.ax = _AX; r->x.bx = _BX; r->x.cx = _CX; r->x.dx = _DX;
    r->x.si = _SI; r->x.di = _DI;
    r->h.al = _AL; r->h.ah = _AH; r->h.bl = _BL; r->h.bh = _BH;
    r->h.cl = _CL; r->h.ch = _CH; r->h.dl = _DL; r->h.dh = _DH;
}

static void handle_int10(union REGS *r)
{
    switch (r->h.ah) {
    case 0x00:
        video_mode = r->h.al;
        break;
    case 0x01:
        break;
    case 0x02:
        text_cursor_row = r->h.dh;
        text_cursor_col = r->h.dl;
        break;
    case 0x03:
        r->h.dh = text_cursor_row;
        r->h.dl = text_cursor_col;
        break;
    case 0x06:
        break;
    case 0x08:
        r->h.al = ' ';
        r->h.ah = 0x07;
        break;
    case 0x09:
        putchar(r->h.al);
        fflush(stdout);
        break;
    case 0x0A:
        putchar(r->h.al);
        fflush(stdout);
        break;
    case 0x0E:
        putchar(r->h.al);
        fflush(stdout);
        break;
    case 0x13:
        break;
    default:
        break;
    }
}

static void handle_int16(union REGS *r)
{
    switch (r->h.ah) {
    case 0x00:
    {
        SDL_Event e;
        while (SDL_WaitEvent(&e)) {
            if (e.type == SDL_EVENT_KEY_DOWN) {
                r->h.al = (unsigned char)(e.key.key & 0xFF);
                r->h.ah = (unsigned char)((e.key.key >> 8) & 0xFF);
                break;
            }
            if (e.type == SDL_EVENT_QUIT) {
                r->h.al = 0x1B;
                r->h.ah = 0x01;
                break;
            }
        }
        break;
    }
    case 0x01:
    {
        SDL_PumpEvents();
        Uint32 mask = SDL_EVENT_KEY_DOWN;
        if (SDL_HasEvents(mask, mask)) {
            r->x.flags &= ~0x40;
        } else {
            r->x.flags |= 0x40;
        }
        break;
    }
    default:
        break;
    }
}

static void handle_int21(union REGS *r)
{
    switch (r->h.ah) {
    case 0x09:
    {
        const char *str = (const char *)(unsigned long)r->x.dx;
        while (*str && *str != '$') {
            putchar(*str);
            str++;
        }
        fflush(stdout);
        break;
    }
    case 0x02:
        putchar(r->h.dl);
        fflush(stdout);
        break;
    case 0x4C:
        exit(r->h.al);
        break;
    case 0x2C:
    {
        time_t t = time(NULL);
        struct tm *tm = localtime(&t);
        r->h.ch = (unsigned char)tm->tm_hour;
        r->h.cl = (unsigned char)tm->tm_min;
        r->h.dh = (unsigned char)tm->tm_sec;
        r->h.dl = 0;
        break;
    }
    case 0x2A:
    {
        time_t t = time(NULL);
        struct tm *tm = localtime(&t);
        r->h.al = (unsigned char)tm->tm_wday;
        r->x.cx = tm->tm_year;
        r->h.dh = (unsigned char)tm->tm_mon + 1;
        r->h.dl = (unsigned char)tm->tm_mday;
        break;
    }
    default:
        break;
    }
}

static void handle_int1A(union REGS *r)
{
    switch (r->h.ah) {
    case 0x00:
    {
        Uint64 ticks = SDL_GetTicks();
        unsigned long dos_ticks = (ticks * 1193180L) / 1000 / 65536;
        r->x.cx = (unsigned short)((dos_ticks >> 16) & 0xFFFF);
        r->x.dx = (unsigned short)(dos_ticks & 0xFFFF);
        r->h.al = 0;
        if (ticks > 86400000) r->h.al = 1;
        break;
    }
    default:
        break;
    }
}

int int86(int intno, union REGS *inregs, union REGS *outregs)
{
    if (!inregs || !outregs) return -1;
    sync_from_regs(inregs);

    switch (intno) {
    case 0x10: handle_int10(inregs); break;
    case 0x16: handle_int16(inregs); break;
    case 0x21: handle_int21(inregs); break;
    case 0x1A: handle_int1A(inregs); break;
    default: break;
    }

    sync_to_regs(outregs);
    return 0;
}

int int86x(int intno, union REGS *inregs, union REGS *outregs, struct SREGS *segregs)
{
    (void)segregs;
    return int86(intno, inregs, outregs);
}

int intdos(union REGS *inregs, union REGS *outregs)
{
    return int86(0x21, inregs, outregs);
}

int intdosx(union REGS *inregs, union REGS *outregs, struct SREGS *segregs)
{
    (void)segregs;
    return int86(0x21, inregs, outregs);
}

void geninterrupt(int intno)
{
    union REGS r = {0};
    int86(intno, &r, &r);
}

void sound(unsigned frequency)
{
    (void)frequency;
}

void nosound(void)
{
}

void sleep(unsigned seconds)
{
    SDL_Delay(seconds * 1000);
}

void _dos_getvect(unsigned intno, void (**vector)(void))
{
    (void)intno;
    if (vector) *vector = NULL;
}

void _dos_setvect(unsigned intno, void (*vector)(void))
{
    (void)intno;
    (void)vector;
}

void keep(unsigned char status, unsigned size)
{
    (void)status;
    (void)size;
}
