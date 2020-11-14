#include <memory.h>

void MemoryPoolInit(MemoryPool *memoryPool, Byte *bits, Uint32 startAddr, Uint32 byteSize)
{
    ASSERT(memoryPool != NULL);
    ASSERT(bits != NULL);
    bitMapInit(&memoryPool->bitMap, bits, byteSize);
    memoryPool->ByteSize = byteSize;
    memoryPool->startAddr = startAddr;
}

void* MemoryPoolAlloc(MemoryPool *memoryPool, Uint32 byteSize)
{
    ASSERT(memoryPool != NULL);
    // byte + 4 代表要多申请 4 Byte 的空间用来存放本次申请的内存大小，用于后续释放。
    int bitIndex = bitMapScanAndSet(&memoryPool->bitMap, byteSize + 4);
    if (bitIndex != -1)
    {
        Uint32 *size = (Uint32*)(memoryPool->startAddr + bitIndex);
        *size = byteSize + 4;
        return (size + 1);
    }
    return NULL;
}

void MemoryPoolFree(MemoryPool *memoryPool, void* startAddr)
{
    ASSERT(memoryPool != NULL);
    ASSERT(startAddr != NULL);
    // 地址 - 4 就是真正的起始地址了
    Int32 bitIndex = (Uint32)(startAddr - 4) - memoryPool->startAddr;
    Uint32 *size = startAddr;
    bitMapUnSetBitRange(&memoryPool->bitMap, bitIndex, *(size - 1));
}