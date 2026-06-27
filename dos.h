#ifndef DOS_H
#define DOS_H

#ifdef __cplusplus
extern "C" {
#endif

union REGS {
    struct {
        unsigned short ax, bx, cx, dx, si, di, cflag, flags;
    } x;
    struct {
        unsigned char al, ah, bl, bh, cl, ch, dl, dh;
    } h;
};

struct SREGS {
    unsigned short es, cs, ss, ds;
};

int int86(int intno, union REGS *inregs, union REGS *outregs);
int int86x(int intno, union REGS *inregs, union REGS *outregs, struct SREGS *segregs);
int intdos(union REGS *inregs, union REGS *outregs);
int intdosx(union REGS *inregs, union REGS *outregs, struct SREGS *segregs);

extern unsigned short _AX, _BX, _CX, _DX, _SI, _DI, _SP, _BP;
extern unsigned char _AL, _AH, _BL, _BH, _CL, _CH, _DL, _DH;

void geninterrupt(int intno);
void delay(int milliseconds);
void sound(unsigned frequency);
void nosound(void);
void sleep(unsigned seconds);

#define FP_OFF(p) ((unsigned long)(p))
#define FP_SEG(p) 0UL
#define MK_FP(seg, off) ((void *)(unsigned long)(off))

void _dos_getvect(unsigned intno, void (**vector)(void));
void _dos_setvect(unsigned intno, void (*vector)(void));
void keep(unsigned char status, unsigned size);

#ifdef __cplusplus
}
#endif

#endif
