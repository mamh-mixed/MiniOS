#include <debug.h>

void panic_spin(const char *filename, int line, const char *func, const char *condition)
{
    interruptDisable();
    puts("\n\n\n!!! error !!! \n");

    puts("filename: ");
    puts(filename);
    puts("\n");

    char str[20];
    puts("line: ");
    puts(itoa(line, str, 10));
    puts("\n");

    puts("function: ");
    puts(func);
    puts("\n");

    puts("condition:");
    puts(condition);
    puts("\n");

    while (1)
    {
    }
}