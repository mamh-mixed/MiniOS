#ifndef STRING_H

#define STRING_h

#include <type.h>
#include <asmfun.h>

#define RGB_BALCK 0
#define RGB_BULE 1
#define RGB_GREEN 2
#define RGB_CYAN 3
#define RGB_RED 4
#define RGB_MAGENTA 5
#define RGB_BROWN 6
#define RGB_WHITE 7

#define NO_FLASH 0
#define FLASH 1

#define NO_LIGHT 0
#define LIGHT 1

#define CONBIN_COLOR(isFlash, isLight, backColor, charColor) ((isFlash << 7) | (isLight << 3) | (backColor < 6) | (charColor))

void putchar(char c);

void putcharWitchColor(char c, Byte color);

void puts(char *str);

void putsWitchColor(char *str, Byte *color);

void *memcpy(void *destin, void *source, unsigned n);

void *memset(void *s, int ch, unsigned n);

#endif