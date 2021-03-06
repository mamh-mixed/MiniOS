#include <stdlib.h>

int atoi(const char *str)
{
    ASSERT(str != NULL);
    int ret = 0;
    for (const char *p = str; *p != '\0'; p++)
    {
        ret = ret * 10 + (*p - '0');
    }
    return ret;
}

char *itoa(int value, char *str, int base)
{
    ASSERT(str != NULL);
    int bak = value; // 备份 value 
    char tempStr[50] = {0}; // 用来记录分解的各个数位，起到一个栈的作用，并且第一个元素是 '\0'。
    int index = 0;
    value = value < 0 ? -value : value;
    if (base == 10)
    {
        if (value == 0)
        {
            tempStr[++index] = '0';
        }
        while (value != 0)
        {
            tempStr[++index] = (value % 10) + '0';
            value /= 10;
        }
        if (bak < 0)
        {
            tempStr[++index] = '-';
        }
    }
    else if (base == 16)
    {
        if (value == 0)
        {
            tempStr[++index] = '0';
        }
        while (value != 0)
        {
            char temp = value % 16;
            if (temp >= 10)
            {
                temp %= 10;
                temp = 'A' + temp;
            }
            else
            {
                temp = '0' + temp;
            }
            tempStr[++index] = temp;
            value /= 16;
        }
        tempStr[++index] = 'x';
        tempStr[++index] = '0';
        if (bak < 0)
        {
            tempStr[++index] = '-';
        }
    }
    for (int i = 0; index >= 0; i++, index--)
    {
        str[i] = tempStr[index];
    }
    return str;
}

char *uitoa(unsigned int value, char *str, int base)
{
    ASSERT(str != NULL);
    unsigned int bak = value; // 备份 value 
    char tempStr[50] = {0}; // 用来记录分解的各个数位，起到一个栈的作用，并且第一个元素是 '\0'。
    int index = 0;
    if (base == 10)
    {
        if (value == 0)
        {
            tempStr[++index] = '0';
        }
        while (value != 0)
        {
            tempStr[++index] = (value % 10) + '0';
            value /= 10;
        }
    }
    else if (base == 16)
    {
        if (value == 0)
        {
            tempStr[++index] = '0';
        }
        while (value != 0)
        {
            char temp = value % 16;
            if (temp >= 10)
            {
                temp %= 10;
                temp = 'A' + temp;
            }
            else
            {
                temp = '0' + temp;
            }
            tempStr[++index] = temp;
            value /= 16;
        }
        tempStr[++index] = 'x';
        tempStr[++index] = '0';
    }
    for (int i = 0; index >= 0; i++, index--)
    {
        str[i] = tempStr[index];
    }
    return str;
}