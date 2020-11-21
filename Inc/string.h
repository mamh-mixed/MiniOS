#ifndef __STRING_H__

#define __STRING_H__ 0

#include <stddef.h>
#include <type.h>
#include <debug.h>

void *memcpy(void *destin, const void *source, unsigned n);

void *memset(void *s, int ch, unsigned n);

int memcmp(const void *a, const void *b, unsigned n);

Uint32 strlen(const char *str);

int strcmp(const char *a, const char *b);

char *strcpy(char *dest, const char *src);

char *strcat(char *pszDest, const char *pszSrc);

char *strchr(const char *s, char c);

#endif