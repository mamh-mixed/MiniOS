#ifndef MEMORY_H

#define MEMORY_H

#include <bitmap.h>

#define PAGE_DIR_BASE_ADDR ((Uint32 *)0xfffff000)

#define KERNEL_VADDR_BYTE_LENGTH (32 * 1024 * 1024 / 8)

#define PADDR_BYTE_LENGTH (0xf0000000 / 4096 / 8)

#define KERNEL_START_VADDR (0x82000000)

#define START_PADDR ((void *)(0x10000000))

typedef struct
{
    BitMap bitMap;
    Uint32 startAddr;
    Uint32 unitCount;
    Uint32 byteSize;
    Uint32 allocUnit;
} MemoryPool;

void initMemoryManagement();

void memoryPoolInit(MemoryPool *memoryPool, Byte *bits, Uint32 startAddr, Uint32 unitCount, Uint32 allocUnit);

void *memoryPoolAlloc(MemoryPool *memoryPool, Uint32 byteSize);

void memoryPoolFree(MemoryPool *memoryPool, void *startAddr);

Bool isPageDescriptorPresent(Uint32 descriptor);

Uint32 makePageDescriptor(void *physicalAddr, Uint32 present, Uint32 rw, Uint32 us, Uint32 pwt, Uint32 pcd, Uint32 gloabl);

void installA4KBPage(Uint32 linearAddr);

void *sysMalloc(Uint32 byteLength);

void sysFree(void *startAddr);

void *phyMalloc4KB();

void phyFree(void *startAddr);

void *malloc(Uint32 byteLength);

void free(void *startAddr);

#endif