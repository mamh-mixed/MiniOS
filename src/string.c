#include <string.h>

void *memcpy(void *destin, const void *source, unsigned n)
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

int memcmp(const void *a, const void *b, unsigned n)
{
    ASSERT(a != NULL);
    ASSERT(b != NULL);
    const Byte *aa = a, *bb = b;
    for (unsigned i = 0; i < n; i++)
    {
        if (aa[i] >= bb[i])
        {
            return 1;
        }
        else if (aa[i] < bb[i])
        {
            return -1;
        }
    }
    return 0;
}

Uint32 strlen(const char *str)
{
    ASSERT(str != NULL);
    Uint32 len = 0;
    while (*str != '\0')
    {
        ++str;
        ++len;
    }
    return len;
}

int strcmp(const char *a, const char *b)
{
    ASSERT(a != NULL);
    ASSERT(b != NULL);
    while (*a != 0 && *a == *b)
    {
        a++;
        b++;
    }
    return *a < *b ? -1 : *a > *b;
}

char *strcpy(char *des, const char *source)
{
    char *r = des;
    ASSERT((des != NULL) && (source != NULL));
    while ((*r++ = *source++) != '\0')
        ;
    return des;
}

char *strcat(char *pszDest, const char *pszSrc)
{
    char *pszOrigDst = pszDest;

    while (*pszDest)
        pszDest++;
    while ((*pszDest++ = *pszSrc++) != '\0')
        ;

    return pszOrigDst;
}

char *strchr(const char *s, char c)
{
    Uint32 len = strlen(s);
    for (Uint32 i = 0; i < len; i++)
    {
        if (s[i] == c)
        {
            return (char *)(&(s[i]));
        }
    }
    return NULL;
}
