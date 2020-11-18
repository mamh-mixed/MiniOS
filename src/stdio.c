#include <stdio.h>

static Semaphore stdoutSemaphore;

void initStdioManagement()
{
    _asm_set_cursor(0);
    semaphoreInit(&stdoutSemaphore, 1);
}

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
        if (cursor + 1 >= 2000)
        {
            memcpy(pVgaMem, pVgaMem + 80 * 2 * 1, 80 * 2 * 24);
            for (int i = 0; i < 80; i++)
            {
                pVgaMem[80 * 2 * 24 + i * 2] = ' ';
                pVgaMem[80 * 2 * 24 + i * 2 + 1] = 0x07;
            }
            cursor = 24 * 80;
        }
    }
    else if (c == 0x0d)
    {
        cursor = (lineNum + 1) * 80;
    }
    else if (c == 0x08)
    {
        cursor = cursor == 0 ? 0 : (cursor - 1);
        pVgaMem[cursor * 2] = ' ';
        pVgaMem[cursor * 2 + 1] = 0x07; 
    }
    else if (cursor + 1 >= 2000)
    {
        pVgaMem[cursor * 2] = c;
        pVgaMem[cursor * 2 + 1] = color;
        memcpy(pVgaMem, pVgaMem + 80 * 2 * 1, 80 * 2 * 24);
        for (int i = 0; i < 80; i++)
        {
            pVgaMem[80 * 2 * 24 + i * 2] = ' ';
            pVgaMem[80 * 2 * 24 + i * 2 + 1] = 0x07;
        }
        cursor = 24 * 80;
    }
    else
    {
        pVgaMem[cursor * 2] = c;
        pVgaMem[cursor * 2 + 1] = color;
        cursor++;
    }

    _asm_set_cursor(cursor);
}

void puts(const char *str)
{
    semaphoreGet(&stdoutSemaphore);
    ASSERT(str != NULL);
    for (const char *p = str; *p != '\0'; p++)
    {
        putchar(*p);
    }
    semaphoreRelease(&stdoutSemaphore);
}

void putsNoSync(const char *str)
{
    ASSERT(str != NULL);
    for (const char *p = str; *p != '\0'; p++)
    {
        putchar(*p);
    }
}

void putDec(int value)
{
    char str[100] = {0};
    itoa(value, str, 10);
    puts(str);
}

void putDecNoSync(int value)
{
    char str[100] = {0};
    itoa(value, str, 10);
    putsNoSync(str);
}

void putUDec(unsigned int value)
{
    char str[100] = {0};
    uitoa(value, str, 10);
    puts(str);
}

void putUDecNoSync(unsigned int value)
{
    char str[100] = {0};
    uitoa(value, str, 10);
    putsNoSync(str);
}

void putHex(int value)
{
    char str[100] = {0};
    itoa(value, str, 16);
    puts(str);
}

void putHexNoSync(int value)
{
    char str[100] = {0};
    itoa(value, str, 16);
    putsNoSync(str);
}

void putUHex(unsigned int value)
{
    char str[100] = {0};
    uitoa(value, str, 16);
    puts(str);
}

void putUHexNoSync(unsigned int value)
{
    char str[100] = {0};
    uitoa(value, str, 16);
    putsNoSync(str);
}