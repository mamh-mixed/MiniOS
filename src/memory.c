#include <memory.h>

void initMemoryManagement()
{
    memoryPoolInit(&kernelVaddrPool, bitsForKernelVaddr, KERNEL_START_VADDR, KERNEL_VADDR_BYTE_LENGTH, 1);
    bitMapInit(&bitMapForPAddr, bitsForPAddr, PADDR_BYTE_LENGTH);
}

void memoryPoolInit(MemoryPool *memoryPool, Byte *bits, Uint32 startAddr, Uint32 unitCount, Uint32 allocUnit)
{
    ASSERT(memoryPool != NULL);
    ASSERT(bits != NULL);
    bitMapInit(&memoryPool->bitMap, bits, unitCount);
    memoryPool->unitCount = unitCount;
    memoryPool->allocUnit = allocUnit;
    memoryPool->startAddr = startAddr;
}

void *memoryPoolAlloc(MemoryPool *memoryPool, Uint32 byteSize)
{
    ASSERT(memoryPool != NULL);
    Uint32 allocCount = (byteSize + 4) / memoryPool->allocUnit;
    if (byteSize % memoryPool->allocUnit != 0)
    {
        allocCount++;
    }

    int bitIndex = bitMapScanAndSet(&memoryPool->bitMap, allocCount);
    ASSERT(bitIndex != -1);
    if (bitIndex != -1)
    {
        Uint32 *size = (Uint32 *)(memoryPool->startAddr + bitIndex * memoryPool->allocUnit);
        *size = allocCount;
        return (size + 1);
    }
    return NULL;
}

void memoryPoolFree(MemoryPool *memoryPool, void *startAddr)
{
    ASSERT(memoryPool != NULL);
    ASSERT(startAddr != NULL);
    Int32 bitIndex = ((Uint32)(startAddr - 4) - memoryPool->startAddr) / memoryPool->allocUnit;
    Uint32 *size = startAddr;
    bitMapUnSetBitRange(&memoryPool->bitMap, bitIndex, *(size - 1));
}

Bool isPageDescriptorPresent(Uint32 descriptor)
{
    return (descriptor & (Uint32)0x00000001) == 0 ? FALSE : TRUE;
}

Uint32 makePageDescriptor(void* physicalAddr, Uint32 present, Uint32 readWrite, Uint32 us, Uint32 pwt, Uint32 pcd, Uint32 gloabl)
{
    Uint32 attr = 0;
    attr |= present == 0 ? 0x00000000 : 0x00000001;
    attr |= readWrite == 0 ? 0x00000000 : 0x00000002;
    attr |= us == 0 ? 0x00000000 : 0x00000004;
    attr |= pwt == 0 ? 0x00000000 : 0x00000008;
    attr |= pcd == 0 ? 0x00000000 : 0x00000010;
    attr |= gloabl == 0 ? 0x00000000 : 0x00000100;
    Uint32 descriptor = attr | ((Uint32)physicalAddr & 0xfffff000);
    return descriptor;
}

void installA4KBPage(Uint32 linearAddr)
{
    ASSERT(linearAddr != 0);
    Uint32 *pageDir = PAGE_DIR_BASE_ADDR;
    Uint32 pageDirIndex = linearAddr >> 22; // 线性地址的高 10 位就是页目录的索引
    Bool needClear = FALSE;
    if (!isPageDescriptorPresent(pageDir[pageDirIndex]))
    {
        needClear = TRUE;
        Uint32 *descriptorAddr = phyMalloc4KB();
        ASSERT(descriptorAddr != NULL);
        Uint32 descriptor = makePageDescriptor(descriptorAddr, 1, 1, 0, 0, 0, 0);
        pageDir[pageDirIndex] = descriptor;
    }

    Uint32 *pageTable = (Uint32*)(0xffc00000 | (pageDirIndex << 12));
    Uint32 pageTableIndex = (linearAddr & 0x003ff000) >> 12;
    if (needClear)
    {
        memset(pageTable, 0, 4096);
    }
    if (!isPageDescriptorPresent(pageTable[pageTableIndex]))
    {
        Uint32 *descriptorAddr = phyMalloc4KB();
        ASSERT(descriptorAddr != NULL);
        Uint32 descriptor = makePageDescriptor(descriptorAddr, 1, 1, 0, 0, 0, 0);
        pageTable[pageTableIndex] = descriptor;
    }
}

void *sysMalloc(Uint32 byteLength)
{
    void* startAddr = memoryPoolAlloc(&kernelVaddrPool, byteLength);
    ASSERT(startAddr != NULL);
    return startAddr;
}

void sysFree(void *startAddr)
{
    ASSERT(startAddr != NULL);
    memoryPoolFree(&kernelVaddrPool, startAddr);
}

void *phyMalloc4KB()
{
    Int32 bitIndex = bitMapScanAndSet(&bitMapForPAddr, 1);
    ASSERT(bitIndex != -1);
    Uint32 startAddr = (Uint32)START_PADDR + bitIndex * 4096;
    return (void*)startAddr;
}

void phyFree(void *startAddr)
{
    ASSERT(startAddr != NULL);
    Uint32 addr = (Uint32)startAddr;
    Uint32 bitIndex = (addr - (Uint32)START_PADDR) / 4096;
    bitMapUnSetBitRange(&bitMapForPAddr, bitIndex, 4096);
}