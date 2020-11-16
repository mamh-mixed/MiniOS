#ifndef STDIO_H

#define STDIO_H

#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <debug.h>
#include <sync.h>

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

// 获取光标的位置
extern unsigned int _asm_get_cursor();

// 设置光标的位置
extern void _asm_set_cursor(Uint16 pos);

void initStdout();

void putchar(const char c);

void putcharWitchColor(const char c, Byte color);

void putsWitchColor(const char *str, Byte *color);

void puts(const char *str);

void putsNoSync(const char *str);

void putDec(int value);

void putDecNoSync(int value);

void putHex(int value);

void putHexNoSync(int value);

void putUDec(unsigned int value);

void putUDecNoSync(unsigned int value);

void putUHex(unsigned int value);

void putUHexNoSync(unsigned int value);

#endif