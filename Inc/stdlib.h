#ifndef STDLIB_H

#define STDLIB_H

#include <stddef.h>
#include <debug.h>

int atoi(const char *str);

char *itoa(int value, char *str, int base);

char *uitoa(unsigned int value, char *str, int base);

#endif