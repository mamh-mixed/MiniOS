#ifndef __TYPE_H__

#define __TYPE_H__ 0

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
    void *baseAddr;
} IDTR;

typedef struct
{
    Uint8 code[200];
    InterruptGateEntry *entry;
} InterruptGateEntryTable;

#pragma pack()

typedef struct
{
    Int32 value;
} Semaphore;

typedef struct _linkListItem
{
    void *data;
    struct _linkListItem *next;
    struct _linkListItem *prev;
} LinkListItem;

typedef struct
{
    Semaphore semaphore;
    LinkListItem *head;
    LinkListItem *tail;
    LinkListItem *cur;
    Uint32 length;
} LinkList;

typedef Bool(LinkListScanCallBack)(LinkListItem *item, void *arg);

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
    Semaphore semaphore;
    void **buf;
    Int32 front;
    Int32 rear;
    Uint32 capacity;
} Queue;

#pragma pack(1)

typedef struct _taskStateSegment
{
    Uint16 prevSelector;
    Uint16 reserve0;

    Uint32 esp0;

    Uint16 ss0;
    Uint16 reserve1;

    Uint32 esp1;

    Uint16 ss1;
    Uint16 reserve2;

    Uint32 esp2;

    Uint16 ss2;
    Uint16 reserve3;

    Uint32 cr3;
    Uint32 eip;
    Uint32 elfags;
    Uint32 eax;
    Uint32 ecx;
    Uint32 edx;
    Uint32 ebx;
    Uint32 esp;
    Uint32 ebp;
    Uint32 esi;
    Uint32 edi;

    Uint16 es;
    Uint16 reserve4;

    Uint16 cs;
    Uint16 reserve5;

    Uint16 ss;
    Uint16 reserve6;

    Uint16 ds;
    Uint16 reserve7;

    Uint16 fs;
    Uint16 reserve8;

    Uint16 gs;
    Uint16 reserve9;

    Uint16 ldtSelector;
    Uint16 reserve10;

    Uint16 reserve11;
    Uint16 ioMapOffset;

} TaskStateSegment;

typedef struct
{
    Uint16 limitLow16;
    Uint16 baseAddrLow16;
    Uint8 baseAddrMid8;
    Uint8 attributeLow8;
    Uint8 limithigh4AndAttributeHight4;
    Uint8 baseAddrHight8;

} GdtDescriptor;

typedef struct
{
    Uint16 limit;
    GdtDescriptor *table;
} GDTR;

#pragma pack()

typedef void(ThreadFunc)();

typedef enum
{
    Running,
    Suspended,
    Ready,
    Blocked,
    Died
} TaskStatus;


typedef struct
{
    Uint32 id;
    const char *name;
    void *args;
    TaskStatus status;
    Uint32 eip;
    Uint32 ss0;
    Uint32 esp0;
    Uint32 ss3;
    Uint32 esp3;
    Uint32 cs;
    Uint32 ds;
    Uint32 fs;
    Uint32 es;
    Uint32 gs;
} Tcb;

typedef struct
{
    Uint32 id;
    const char *name;
    TaskStatus status;
    Uint32 cr3;
    Uint32 dpl;
    MemoryPool memoryPool;
    LinkList threadList;
    Int32 openedFcb[8];
} Pcb;

typedef enum
{
    GetPid, // 0
    Open, // 1
    Close, // 2
    Create, // 3
    Read, // 4
    Write, // 5
    Delete, // 6
    SuspendProcess, // 7
    BlockProcess, // 8
    ExitProcess, // 9
    SuspendThread, // 10
    BlockThread, // 11
    ExitThread // 12
} SysCall;

typedef enum
{
    MiniFS
} FileSystemType;

typedef enum
{
    Stdin,
    Stdout,
    GernalFile,
    Directory
} FileType;

typedef enum
{
    GernalOpen,
    MapOpen
} OpenMode;

#pragma pack(1)

typedef struct
{
    char magic[10];
    FileSystemType fileSystemType;
    Uint32 fileDescriptorCount;
    Uint32 fileDescriptorMaxCount;
    Uint32 fileDescriptorStartSector;
    Uint32 fileStartSector;
} FileSystemInfo;

typedef struct
{
    FileType fileType;
    Bool isExists;
    Bool isOpen;
    Bool isMaped;
    Bool isInMemory;
    char filename[256];
    Uint32 startSector;
    Uint32 sectorCount;
    Uint32 byteSize;
    void* mapAddr;
    Uint32 fileOffset;
    Uint32 mapLength;
    Byte padding[212]; // 凑够 512 Byte
} Fcb;

#pragma pack()

#endif