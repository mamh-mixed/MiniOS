#ifndef STDDEF_H

#define STDDEF_H

#define NULL ((void *)0)

#define GDT_SELECTOR_DPL_0_4GB_CODE 0x10

#define GDT_SELECTOR_DPL_0_4GB_DATA 0x08

typedef unsigned char Byte;

typedef unsigned char Uint8;

typedef unsigned short Uint16;

typedef unsigned int Uint32;

typedef char Int8;

typedef short Int16;

typedef int Int32;

#endif