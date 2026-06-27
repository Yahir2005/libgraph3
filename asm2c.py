#!/usr/bin/env python3
"""
asm2c.py — Preprocesador para convertir bloques asm{...} de Turbo C
a llamadas int86() compatibles con dos.h.

Uso:  python3 asm2c.py entrada.c > salida.c

Ejemplo de entrada:
    asm{mov ax,0013h,mov bh,00,int 10h}

Salida:
    { union REGS __r = {0};
      __r.h.ah = 0x00; __r.h.al = 0x13;
      __r.h.bh = 0x00;
      int86(0x10, &__r, &__r); }
"""

import re
import sys

MNEMONICS = r'\b(?:mov|int|add|sub|xor|and|or|inc|dec|not|neg|shl|shr|cmp|xchg|push|pop|mul|div|imul|idiv|call|ret|jmp|jz|jnz|je|jne|jl|jg|jle|jge|ja|jb|jae|jbe|jc|jnc|jo|jno|js|jns|jp|jnp|loop|loope|loopne)\b'

REGS_MAP = {
    'ax': '.x.ax', 'bx': '.x.bx', 'cx': '.x.cx', 'dx': '.x.dx',
    'si': '.x.si', 'di': '.x.di', 'bp': '.x.bp', 'sp': '.x.sp',
    'al': '.h.al', 'ah': '.h.ah', 'bl': '.h.bl', 'bh': '.h.bh',
    'cl': '.h.cl', 'ch': '.h.ch', 'dl': '.h.dl', 'dh': '.h.dh',
}


def clean_val(s):
    s = s.strip()
    while s and s[-1] in ';,':
        s = s[:-1].strip()
    return s

def hex2c(val):
    val = clean_val(val).lower()
    if val.endswith('h') and not val.startswith('0x'):
        val = '0x' + val[:-1]
    elif val.endswith('b') and not val.startswith('0b'):
        val = '0b' + val[:-1]
    return val


def reg_access(name):
    return REGS_MAP.get(name.lower().strip())


def convert_asm(content):
    parts = re.split(r'(?=' + MNEMONICS + r')', content, flags=re.IGNORECASE)
    parts = [p.strip() for p in parts if p.strip()]

    lines = []
    lines.append('{ union REGS __r = {0};')

    for part in parts:
        part = part.rstrip(',').strip()
        if not part:
            continue
        m = re.match(MNEMONICS, part, re.IGNORECASE)
        if not m:
            continue
        mne = m.group(0).lower()
        rest = part[m.end():].strip().rstrip(',').strip()
        if not rest and mne != 'ret':
            continue

        if mne == 'mov':
            ops = [o.strip() for o in rest.split(',')]
            if len(ops) == 2:
                dst, src = clean_val(ops[0]), clean_val(ops[1])
                acc = reg_access(dst)
                src_acc = reg_access(src)
                if acc and src_acc:
                    lines.append(f'  __r{acc} = __r{src_acc};')
                elif acc:
                    lines.append(f'  __r{acc} = {hex2c(src)};')
        elif mne == 'int':
            num = hex2c(rest.split()[0])
            lines.append(f'  int86({num}, &__r, &__r);')
        elif mne in ('add', 'sub', 'xor', 'and', 'or'):
            ops = [o.strip() for o in rest.split(',')]
            if len(ops) == 2:
                dst, src = clean_val(ops[0]), clean_val(ops[1])
                acc = reg_access(dst)
                src_acc = reg_access(src)
                if acc:
                    opmap = {'add': '+=', 'sub': '-=', 'xor': '^=', 'and': '&=', 'or': '|='}
                    if src_acc:
                        lines.append(f'  __r{acc} {opmap[mne]} __r{src_acc};')
                    else:
                        lines.append(f'  __r{acc} {opmap[mne]} {hex2c(src)};')
        elif mne == 'inc':
            acc = reg_access(rest)
            if acc:
                lines.append(f'  __r{acc}++;')
        elif mne == 'dec':
            acc = reg_access(rest)
            if acc:
                lines.append(f'  __r{acc}--;')
        elif mne == 'not':
            acc = reg_access(rest)
            if acc:
                lines.append(f'  __r{acc} = ~__r{acc};')
        elif mne == 'neg':
            acc = reg_access(rest)
            if acc:
                lines.append(f'  __r{acc} = -__r{acc};')
        elif mne == 'cmp':
            ops = [o.strip() for o in rest.split(',')]
            if len(ops) == 2:
                dst, src = clean_val(ops[0]), clean_val(ops[1])
                acc_dst = reg_access(dst)
                acc_src = reg_access(src)
                if acc_dst:
                    if acc_src:
                        lines.append(f'  __r{acc_dst} -= __r{acc_src};')
                    else:
                        lines.append(f'  __r{acc_dst} -= {hex2c(src)};')
        elif mne == 'xchg':
            ops = [o.strip() for o in rest.split(',')]
            if len(ops) == 2:
                a1, a2 = reg_access(ops[0]), reg_access(ops[1])
                if a1 and a2:
                    lines.append(f'  {{ unsigned short __t = __r{a1}; __r{a1} = __r{a2}; __r{a2} = __t; }}')
        elif mne in ('call', 'jmp', 'jz', 'jnz', 'je', 'jne', 'jl', 'jg', 'jle', 'jge',
                     'ja', 'jb', 'jae', 'jbe', 'jc', 'jnc', 'jo', 'jno', 'js', 'jns',
                     'jp', 'jnp', 'loop', 'loope', 'loopne'):
            lines.append(f'  /* {mne} {rest} — no implementado en emulacion */')
        elif mne in ('push', 'pop'):
            lines.append(f'  /* {mne} {rest} — no implementado en emulacion */')
        elif mne == 'ret':
            lines.append('  /* ret — no implementado en emulacion */')

    lines.append('}')
    return '\n'.join(lines)


def convert_file(text):
    placeholders = {}
    counter = [0]

    def save(s):
        key = f'__ASM2C_COMMENT_{counter[0]}__'
        counter[0] += 1
        placeholders[key] = s
        return key

    text = re.sub(r'//[^\n]*', lambda m: save(m.group(0)), text)
    text = re.sub(r'/\*.*?\*/', lambda m: save(m.group(0)), text, flags=re.DOTALL)
    text = re.sub(r"'[^']*'", lambda m: save(m.group(0)), text)
    text = re.sub(r'"[^"]*"', lambda m: save(m.group(0)), text)

    pattern = r'asm\s*\{([^}]*)\}'
    result = re.sub(pattern, lambda m: convert_asm(m.group(1)), text, flags=re.DOTALL)

    for key, val in placeholders.items():
        result = result.replace(key, val)
    return result


def main():
    if len(sys.argv) < 2:
        print("Uso: asm2c.py <archivo.c>", file=sys.stderr)
        print("Convierte bloques asm{...} a llamadas int86()", file=sys.stderr)
        sys.exit(1)
    with open(sys.argv[1], 'r') as f:
        source = f.read()
    sys.stdout.write(convert_file(source))


if __name__ == '__main__':
    main()
