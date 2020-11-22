#include <memory.h>

// 内核占有的 32 MB 内存对应的位图，1bit 对应 1Byte。
static Byte bitsForKernelVaddr[KERNEL_VADDR_BYTE_LENGTH];

// 高 3.9 GB 的物理内存位图。1bit 对应 4096KB
static Byte bitsForPAddr[PADDR_BYTE_LENGTH];
static BitMap bitMapForPAddr;

// 内核虚拟地址池
static MemoryPool kernelVaddrPool;

// 高 3.9 GB 物理地址池
static MemoryPool physicalAddrPool;

void initMemoryManagement()
{
    memoryPoolInit(&kernelVaddrPool, bitsForKernelVaddr, KERNEL_START_VADDR, KERNEL_VADDR_BYTE_LENGTH, 4096);
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
    if ((byteSize + 4) % memoryPool->allocUnit != 0)
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

Bool isPageDescriptorExists(Uint32 descriptor)
{
    return (descriptor & (Uint32)0xfffff000) == 0 ? FALSE : TRUE;
}

Uint32 makePageDescriptor(void *physicalAddr, Uint32 present, Uint32 readWrite, Uint32 us, Uint32 pwt, Uint32 pcd, Uint32 gloabl, Uint32 avl)
{
    Uint32 attr = 0;
    attr |= present == 0 ? 0x00000000 : 0x00000001;
    attr |= readWrite == 0 ? 0x00000000 : 0x00000002;
    attr |= us == 0 ? 0x00000000 : 0x00000004;
    attr |= pwt == 0 ? 0x00000000 : 0x00000008;
    attr |= pcd == 0 ? 0x00000000 : 0x00000010;
    attr |= gloabl == 0 ? 0x00000000 : 0x00000100;
    attr |= avl << 9;
    Uint32 descriptor = attr | ((Uint32)physicalAddr & 0xfffff000);
    return descriptor;
}

void installA4KBPage(void *destPageDirPhyAddr, Uint32 linearAddr, Uint32 present,Uint32 us)
{
    ASSERT(linearAddr != 0);
    Uint32 *pageDir = PAGE_DIR_BASE_ADDR;

    Uint32 tempDescriptor = makePageDescriptor(destPageDirPhyAddr, present, 1, us, 0, 0, 0, 0);
    pageDir[1022] = tempDescriptor;

    Uint32 *destPageDir = (Uint32 *)0xffffe000;
    Uint32 pageDirIndex = linearAddr >> 22; // 线性地址的高 10 位就是页目录的索引
    Bool needClear = FALSE;
    if (!isPageDescriptorExists(destPageDir[pageDirIndex]))
    {
        needClear = TRUE;
        Uint32 *descriptorAddr = phyMalloc4KB();
        ASSERT(descriptorAddr != NULL);
        Uint32 descriptor = makePageDescriptor(descriptorAddr, present, 1, us, 0, 0, 0, 0);
        destPageDir[pageDirIndex] = descriptor;
    }

    Uint32 *pageTable = (Uint32 *)(0xff800000 | (pageDirIndex << 12));
    Uint32 pageTableIndex = (linearAddr & 0x003ff000) >> 12;
    if (needClear)
    {
        memset(pageTable, 0, 4096);
    }
    if (!isPageDescriptorExists(pageTable[pageTableIndex]))
    {
        Uint32 *descriptorAddr = phyMalloc4KB();
        ASSERT(descriptorAddr != NULL);
        Uint32 descriptor = makePageDescriptor(descriptorAddr, present, 1, us, 0, 0, 0, 0);
        pageTable[pageTableIndex] = descriptor;
    }

    pageDir[1022] = 0;
    memset((void *)linearAddr, 0, 0x1000);

    // 刷新 TLB
    asm volatile(
        "mov %%cr3,%%eax; \
         mov %%eax,%%cr3;" ::
            : "eax");
}

void freeUserPage(void *destPageDirPhyAddr)
{
    Uint32 *curPageDir = PAGE_DIR_BASE_ADDR;
    Uint32 tempDescriptor = makePageDescriptor(destPageDirPhyAddr, 1, 1, 0, 0, 0, 0, 0);
    curPageDir[1022] = tempDescriptor;

    Uint32 *destPageDir = (Uint32 *)0xffffe000;
    for (Uint32 i = 0; i < 512; i++)
    {
        // 如果当前的页目录项存在
        if (isPageDescriptorExists(destPageDir[i]))
        {
            Uint32 *pageTable = (Uint32 *)((Uint32)0xffe00000 | (i << 12));
            for (Uint32 j = 0; j < 1024; j++)
            {
                // 如果当前的页表项存在
                if (isPageDescriptorExists(pageTable[j]))
                {
                    phyFree((void *)GET_BASE_ADDR(pageTable[j]));
                }
            }
            phyFree((void *)GET_BASE_ADDR(destPageDir[i]));
        }
    }
    phyFree(destPageDirPhyAddr);
}

Uint32 initAUserPageDir()
{
    void *phyAddr = phyMalloc4KB(); // 申请一个物理页
    ASSERT(phyAddr != NULL);
    Uint32 *pageDir = PAGE_DIR_BASE_ADDR;                              // 页目录基址
    Uint32 descriptor = makePageDescriptor(phyAddr, 1, 1, 0, 0, 0, 0, 0); // 页目录内容依然敏感，所以仅 ring0 能访问。
    pageDir[1022] = descriptor;                                        // 借用页目录的倒数第二项，用来初始化新的页目录。

    Uint32 *newPageDir = (Uint32 *)0xffffe000; // 此地址指向新的页目录基址
    newPageDir[1023] = descriptor;

    // 拷贝高 2GB 的页目录到新的页目录中，但是不拷贝最后两项，因为其有特殊用途，通过最后两项可以修改页目录和创建新的页目录。
    for (int i = 512; i < 1023; i++)
    {
        newPageDir[i] = pageDir[i];
    }
    pageDir[1022] = 0;
    return (Uint32)phyAddr;
}

void *sysMalloc(Uint32 byteLength)
{
    void *startAddr = memoryPoolAlloc(&kernelVaddrPool, byteLength);
    ASSERT(startAddr != NULL);
    memset(startAddr, 0, byteLength);
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
    return (void *)startAddr;
}

void phyFree(void *startAddr)
{
    ASSERT(startAddr != NULL);
    Uint32 addr = (Uint32)startAddr;
    Uint32 bitIndex = (addr - (Uint32)START_PADDR) / 4096;
    bitMapUnSetBitRange(&bitMapForPAddr, bitIndex, 4096);
}