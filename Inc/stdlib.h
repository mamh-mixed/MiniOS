#ifndef __STDLIB_H__

#define __STDLIB_H__

#include <stddef.h>
#include <type.h>
#include <debug.h>

int atoi(const char *str);

char *itoa(int value, char *str, int base);

char *uitoa(unsigned int value, char *str, int base);

#endif