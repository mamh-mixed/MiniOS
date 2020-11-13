#include <stdlib.h>

int atoi(const char *str)
{
    int ret = 0;
    for (const char *p = str; *p != '\0'; p++)
    {
        ret = ret * 10 + (*p - '0');
    }
    return ret;
}

char *itoa(int value, char *str, int base)
{
    int bak = value;
    char temp[50] = {0};
    int index = 0;
    if (base == 10)
    {
        while (value != 0)
        {
            temp[++index] = (value % 10) + '0';
            value /= 10;
        }
        if (bak < 0) {
            temp[++index] = '-';
        }
    } else if (base == 16) {
        while (value != 0)
        {
            temp[++index] = (value % 16) + '0';
            value /= 16;
        }
        temp[++index] = 'x';
        temp[++index] = '0';
        if (bak < 0) {
            temp[++index] = '-';
        }
    }
    for (unsigned i = 0; index >= 0; i++, index--)
    {
        str[i] = temp[index];
    }
    return str;
}