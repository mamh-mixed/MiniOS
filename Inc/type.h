#ifndef __TYPE_H__

#define __TYPE_H__

#include <stddef.h>

typedef struct
{
    Byte *bits;
    Uint32 byteLength;
} BitMap;

typedef enum
{
    INTERRUPT_OFF,
    INTERRUPT_ON
} InterruptStatus;

typedef void *InterruptGateEntry;

#pragma pack(1)
typedef struct
{
    Uint16 entryLow16;
    Uint16 entrySelector;
    Uint8 dcount;

    Uint8 attribute;
    Uint16 entryHight16;

} InterruptGateDescriptor;

typedef struct
{
    Uint16 limit;
    void* baseAddr;
} IDTR;

typedef struct
{
    Uint8 code[200];
    InterruptGateEntry *entry;
} InterruptGateEntryTable;

#pragma pack()

typedef struct _linkListItem
{
    void *data;
    struct _linkListItem *next;
    struct _linkListItem *prev;
} LinkListItem;

typedef struct
{
    LinkListItem head;
    LinkListItem tail;
} LinkList;

typedef int(function)(LinkListItem *, int arg);

typedef struct
{
    BitMap bitMap;
    Uint32 startAddr;
    Uint32 unitCount;
    Uint32 byteSize;
    Uint32 allocUnit;
} MemoryPool;

typedef struct
{
    Int32 value;
} Semaphore;

typedef struct
{
    Semaphore semaphore;
    void **buf;
    Int32 front;
    Int32 rear;
    Uint32 capacity;
} Queue;


#endif