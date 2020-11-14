#ifndef MEMORY_H

#define MEMORY_H

#include <bitmap.h>

typedef struct
{
    BitMap bitMap;
    Uint32 startAddr;
    Uint32 ByteSize;
} MemoryPool;

void MemoryPoolInit(MemoryPool *memoryPool, Byte *bits, Uint32 startAddr, Uint32 byteSize);

void* MemoryPoolAlloc(MemoryPool *memoryPool, Uint32 byteSize);

void MemoryPoolFree(MemoryPool *memoryPool, void* startAddr);

#endif