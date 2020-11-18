#include <debug.h>

void panic_spin(const char *filename, int line, const char *func, const char *condition)
{
    interruptDisable();
    putsNoSync("\n\n\n!!! error !!! \n");

    putsNoSync("filename: ");
    putsNoSync(filename);
    putsNoSync("\n");

    putsNoSync("line: ");
    putDecNoSync(line);
    putsNoSync("\n");

    putsNoSync("function: ");
    putsNoSync(func);
    putsNoSync("\n");

    putsNoSync("condition: ");
    putsNoSync(condition);
    putsNoSync("\n");

    while (1)
    {
    }
}