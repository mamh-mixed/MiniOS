#include <string.h>

void putchar(const char c)
{
    Byte color = 0x9;
    putcharWitchColor(c, CONBIN_COLOR(NO_FLASH, NO_LIGHT, RGB_BALCK, RGB_WHITE));
}

void putcharWitchColor(const char c, Byte color)
{
    // 显存首地址
    unsigned char *pVgaMem = (unsigned char *)0x000b8000;
    Uint32 cursor = _asm_get_cursor();
    Uint32 lineNum = cursor / 80;
    if (c == '\n')
    {
        cursor = (lineNum + 1) * 80;
    }
    else if (c == 0x0d)
    {
        cursor = lineNum * 80;
    }
    else if (cursor + 1 >= 2000)
    {
        memcpy(pVgaMem, pVgaMem + 80 * 2, 80 * 2 * 24);
        cursor = 24 * 80;
        pVgaMem[cursor * 2] = c;
        pVgaMem[cursor * 2 + 1] = color;
        cursor++;
    } else {
        pVgaMem[cursor * 2] = c;
        pVgaMem[cursor * 2 + 1] = color;
        cursor++;
    }
    
    _asm_set_cursor(cursor);
}

void puts(const char *str)
{
    ASSERT(str != NULL);
    for (const char *p = str; *p != '\0'; p++)
    {
        putchar(*p);
    }
}

void *memcpy(void *destin, void *source, unsigned n)
{
    ASSERT(destin != NULL);
    ASSERT(source != NULL);
    char *dest = (char *)destin;
    char *src = (char *)source;
    for (unsigned i = 0; i < n; i++)
    {
        dest[i] = src[i];
    }
    return destin;
}

void *memset(void *s, int ch, unsigned n)
{
    ASSERT(s != NULL);
    char *dest = s;
    for (unsigned i = 0; i < n; i++)
    {
        dest[i] = ch;
    }
    return s;
}

