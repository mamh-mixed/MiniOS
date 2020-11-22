#include <filesys.h>

static FileSystemInfo *fileSystemInfo;
static Fcb *fcbTable = NULL;
static BitMap sectorMap;
static Byte bitsForSectorMap[FILE_SYSTEM_FILE_SECTOR_COUNT / 8];

void initFileSystem()
{
    fileSystemInfo = (FileSystemInfo *)sysMalloc(ALIGN_512(sizeof(FileSystemInfo)));
    fcbTable = (Fcb *)sysMalloc(512 * FILE_SYSTEM_MAX_FILE_COUNT);
    ASSERT(fileSystemInfo != NULL);
    ASSERT(fcbTable != NULL);
    readFileSystemInfo();
    if (strcmp(fileSystemInfo->magic, FILE_SYSTEM_MAGIC))
    {
        formatDisk();
    }
    readAllFcb();
    initBitmap();
}

void initBitmap()
{
    ASSERT(fileSystemInfo != NULL);
    ASSERT(strcmp(fileSystemInfo->magic, FILE_SYSTEM_MAGIC) == 0);
    ASSERT(fcbTable != NULL);
    bitMapInit(&sectorMap, bitsForSectorMap, FILE_SYSTEM_FILE_SECTOR_COUNT / 8);
    for (Uint32 i = 0; i < fileSystemInfo->fileDescriptorCount; i++)
    {
        if (fcbTable[i].isExists)
        {
            Uint32 bitIndex = fcbTable[i].startSector - fileSystemInfo->fileStartSector;
            bitMapSetBitRange(&sectorMap, bitIndex, fcbTable[i].sectorCount);
        }
    }
}

void formatDisk()
{
    ASSERT(fileSystemInfo != NULL);
    strcpy(fileSystemInfo->magic, FILE_SYSTEM_MAGIC);
    fileSystemInfo->fileDescriptorStartSector = FILE_SYSTEM_FILE_DESCRIPTOR_START_SECTOR;
    fileSystemInfo->fileStartSector = File_SYSTEM_FILE_START_SECTOR;
    fileSystemInfo->fileDescriptorMaxCount = FILE_SYSTEM_MAX_FILE_COUNT;
    createStdin();
    createStdout();
    writeBackToDisk();
}

void readFileSystemInfo()
{
    ASSERT(fileSystemInfo != NULL);
    readDisk(FILE_SYSTEM_INFO_SECTOR, ALIGN_512(sizeof(FileSystemInfo)), fileSystemInfo);
}

void writeFileSystemInfo()
{
    ASSERT(fileSystemInfo != NULL);
    ASSERT(strcmp(fileSystemInfo->magic, FILE_SYSTEM_MAGIC) == 0);
    Uint32 sectorCount = ALIGN_512(sizeof(FileSystemInfo)) / 512;
    writeDisk(FILE_SYSTEM_INFO_SECTOR, sectorCount, fileSystemInfo);
}

void readAllFcb()
{
    ASSERT(fileSystemInfo != NULL);
    ASSERT(strcmp(fileSystemInfo->magic, FILE_SYSTEM_MAGIC) == 0);
    ASSERT(fcbTable != NULL);
    Uint32 sector = fileSystemInfo->fileDescriptorStartSector;
    Uint32 sectorCount = (ALIGN_512(sizeof(Fcb)) / 512);
    Uint32 index = 0;
    for (; index < fileSystemInfo->fileDescriptorCount; index++)
    {
        readDisk(sector, sectorCount, &fcbTable[index]);
        ASSERT(fcbTable[index].isOpen == FALSE)
        sector += sectorCount;
    }
    for (; index < fileSystemInfo->fileDescriptorMaxCount; index++)
    {
        fcbTable[index].isExists = FALSE;
        fcbTable[index].isOpen = FALSE;
    }
}

void writeAllFcb()
{
    ASSERT(fileSystemInfo != NULL);
    ASSERT(strcmp(fileSystemInfo->magic, FILE_SYSTEM_MAGIC) == 0);
    ASSERT(fcbTable != NULL);
    Uint32 sector = fileSystemInfo->fileDescriptorStartSector;
    Uint32 sectorCount = (ALIGN_512(sizeof(Fcb)) / 512);
    for (Uint32 i = 0; i < fileSystemInfo->fileDescriptorMaxCount; i++)
    {
        if (fcbTable[i].isExists)
        {
            Fcb oldFcb;
            memcpy(&oldFcb, &fcbTable[i], sizeof(Fcb));
            fcbTable[i].isOpen = FALSE;
            fcbTable[i].isMaped = FALSE;
            fcbTable[i].isInMemory = FALSE;
            writeDisk(sector, sectorCount, &fcbTable[i]);
            memcpy(&fcbTable[i], &oldFcb, sizeof(Fcb));
            sector += sectorCount;
        }
    }
}

void writeBackToDisk()
{
    writeFileSystemInfo();
    writeAllFcb();
}

void createStdin()
{
    ASSERT(fileSystemInfo != NULL);
    ASSERT(strcmp(fileSystemInfo->magic, FILE_SYSTEM_MAGIC) == 0);
    ASSERT(fcbTable != NULL);
    (fileSystemInfo->fileDescriptorCount)++;
    fcbTable[0].fileType = Stdin;
    fcbTable[0].startSector = File_SYSTEM_FILE_START_SECTOR;
    fcbTable[0].sectorCount = 1;
    fcbTable[0].isExists = TRUE;
    fcbTable[0].isOpen = FALSE;
    fcbTable[0].isMaped = FALSE;
    strcpy(fcbTable[0].filename, "stdin");
}

void createStdout()
{
    ASSERT(fileSystemInfo != NULL);
    ASSERT(strcmp(fileSystemInfo->magic, FILE_SYSTEM_MAGIC) == 0);
    ASSERT(fcbTable != NULL);
    (fileSystemInfo->fileDescriptorCount)++;
    fcbTable[1].fileType = Stdout;
    fcbTable[1].isExists = TRUE;
    fcbTable[1].isOpen = FALSE;
    fcbTable[1].isMaped = FALSE;
    fcbTable[1].startSector = File_SYSTEM_FILE_START_SECTOR + 1;
    fcbTable[1].sectorCount = 1;
    strcpy(fcbTable[1].filename, "stdout");
}

Uint32 allocSector(Uint32 sectorCount)
{
    ASSERT(fileSystemInfo != NULL);
    ASSERT(strcmp(fileSystemInfo->magic, FILE_SYSTEM_MAGIC) == 0);
    ASSERT(fcbTable != NULL);
    return bitMapScanAndSet(&sectorMap, sectorCount) + fileSystemInfo->fileStartSector + 2;
}

void freeSector(Uint32 startSector, Uint32 sectorCount)
{
    ASSERT(fileSystemInfo != NULL);
    ASSERT(strcmp(fileSystemInfo->magic, FILE_SYSTEM_MAGIC) == 0);
    ASSERT(fcbTable != NULL);
    Uint32 bitIndex = startSector - fileSystemInfo->fileStartSector - 2;
    bitMapUnSetBitRange(&sectorMap, bitIndex, sectorCount);
}

Int32 findFileDescriptor(const char *filename)
{
    ASSERT(fileSystemInfo != NULL);
    ASSERT(strcmp(fileSystemInfo->magic, FILE_SYSTEM_MAGIC) == 0);
    ASSERT(fcbTable != NULL);
    for (Uint32 i = 0; i < fileSystemInfo->fileDescriptorMaxCount; i++)
    {
        if (!strcmp(fcbTable[i].filename, filename) && fcbTable[i].isExists)
        {
            return i;
        }
    }
    return -1;
}

Int32 findFreeFileDescriptor()
{
    ASSERT(fileSystemInfo != NULL);
    ASSERT(strcmp(fileSystemInfo->magic, FILE_SYSTEM_MAGIC) == 0);
    ASSERT(fcbTable != NULL);
    for (Uint32 i = 0; i < fileSystemInfo->fileDescriptorMaxCount; i++)
    {
        if (fcbTable[i].isExists == FALSE)
        {
            return i;
        }
    }
    return -1;
}

Uint32 getFileList(char *buffer)
{
    ASSERT(fileSystemInfo != NULL);
    ASSERT(strcmp(fileSystemInfo->magic, FILE_SYSTEM_MAGIC) == 0);
    ASSERT(fcbTable != NULL);
    ASSERT(buffer != NULL);
    buffer[0] = '\0';
    Uint32 count = 0;
    for (Uint32 i = 0; i < fileSystemInfo->fileDescriptorMaxCount; i++)
    {
        if (fcbTable[i].isExists)
        {
            strcat(buffer, fcbTable[i].filename);
            strcat(buffer, "\n");
        }
    }
}

Fcb *getFcb(Uint32 index)
{
    return &fcbTable[index];
}

Bool openFile(const char *filename, OpenMode mode, Uint32 fileOffset, Uint32 mapLength)
{
    ASSERT(fileSystemInfo != NULL);
    ASSERT(strcmp(fileSystemInfo->magic, FILE_SYSTEM_MAGIC) == 0);
    ASSERT(fcbTable != NULL);
    ASSERT(filename != NULL);
    ASSERT(fileOffset % 512 == 0);
    ASSERT(mapLength % 512 == 0);

    Int32 index = findFileDescriptor(filename);
    if (index == -1)
    {
        return FALSE;
    }
    fcbTable[index].isOpen = TRUE;
    fcbTable[index].isMaped = FALSE;

    if (mode == MapOpen)
    {
        fcbTable[index].isMaped = TRUE;
        fcbTable[index].isInMemory = FALSE;
        fcbTable[index].fileOffset = fileOffset;
        fcbTable[index].mapLength = mapLength;
        Pcb *curPcb = getCurPcb();
        fcbTable[index].mapAddr = memoryPoolAlloc(&curPcb->memoryPool, mapLength);
    }

    return appendToPcb(getCurPcb(), index);
}

Bool closeFile(const char *filename)
{
    ASSERT(fileSystemInfo != NULL);
    ASSERT(strcmp(fileSystemInfo->magic, FILE_SYSTEM_MAGIC) == 0);
    ASSERT(fcbTable != NULL);
    ASSERT(filename != NULL);

    Int32 index = findFileDescriptor(filename);
    if (index == -1)
    {
        return FALSE;
    }
    fcbTable[index].isOpen = FALSE;

    if (fcbTable[index].isMaped)
    {
        Pcb *curPcb = getCurPcb();
        Uint32 starSector = (fcbTable[index].fileOffset - fcbTable[index].byteSize) / 512;
        writeDisk(starSector, fcbTable[index].mapLength / 512, fcbTable[index].mapAddr);
        memoryPoolFree(&curPcb->memoryPool, fcbTable[index].mapAddr);
        fcbTable[index].mapAddr = NULL;
        fcbTable[index].isInMemory = FALSE;
    }

    fcbTable[index].isMaped = FALSE;
    return deleteFromPcb(getCurPcb(), index);
}

Bool createFile(const char *filename, const FileType fileType)
{
    ASSERT(fileSystemInfo != NULL);
    ASSERT(strcmp(fileSystemInfo->magic, FILE_SYSTEM_MAGIC) == 0);
    ASSERT(fcbTable != NULL);
    ASSERT(filename != NULL);

    Int32 index = findFileDescriptor(filename);
    if (index != -1)
    {
        return FALSE;
    }
    index = findFreeFileDescriptor();
    if (index == -1)
    {
        return FALSE;
    }

    fcbTable[index].fileType = fileType;
    fcbTable[index].isExists = TRUE;
    fcbTable[index].isOpen = FALSE;
    fcbTable[index].isMaped = FALSE;
    fcbTable[index].isInMemory = FALSE;
    strcpy(fcbTable[index].filename, filename);
    fcbTable[index].startSector = allocSector(1);
    fcbTable[index].sectorCount = 1;
    fcbTable[index].byteSize = 0;
    (fileSystemInfo->fileDescriptorCount)++;
    writeBackToDisk();
    return TRUE;
}

Bool readFile(const char *filename, void *buffer)
{
    ASSERT(fileSystemInfo != NULL);
    ASSERT(strcmp(fileSystemInfo->magic, FILE_SYSTEM_MAGIC) == 0);
    ASSERT(fcbTable != NULL);
    ASSERT(filename != NULL);
    ASSERT(buffer != NULL);

    Int32 index = findFileDescriptor(filename);
    if (index == -1)
    {
        return FALSE;
    }

    if (!fcbTable[index].isOpen)
    {
        return FALSE;
    }
    if (fcbTable[index].fileType == Stdin)
    {
        char c = getchar();
        if (c != '\0')
        {
            ((char *)buffer)[0] = c;
            return TRUE;
        }
        return FALSE;
    }
    else
    {
        readDisk(fcbTable[index].startSector, fcbTable[index].sectorCount, buffer);
    }

    return TRUE;
}

Bool writeFile(const char *filename, void *buffer, Uint32 bufferSize)
{
    ASSERT(fileSystemInfo != NULL);
    ASSERT(strcmp(fileSystemInfo->magic, FILE_SYSTEM_MAGIC) == 0);
    ASSERT(fcbTable != NULL);
    ASSERT(filename != NULL);
    ASSERT(buffer != NULL);
    ASSERT(bufferSize != 0);
    ASSERT(bufferSize % 512 == 0);

    Int32 index = findFileDescriptor(filename);
    if (index == -1)
    {
        return FALSE;
    }
    if (!fcbTable[index].isOpen)
    {
        return FALSE;
    }
    if (fcbTable[index].fileType == Stdout)
    {
        putsNoSync((char *)buffer);
    }
    else
    {
        if (bufferSize > fcbTable[index].sectorCount * 512)
        {
            Uint32 sectorCount = bufferSize / 512;
            freeSector(fcbTable[index].startSector, fcbTable[index].sectorCount);
            fcbTable[index].startSector = allocSector(sectorCount);
            fcbTable[index].sectorCount = sectorCount;
        }
        writeDisk(fcbTable[index].startSector, fcbTable[index].sectorCount, buffer);
        writeBackToDisk();
    }
    return TRUE;
}

Bool deleteFile(const char *filename)
{
    ASSERT(fileSystemInfo != NULL);
    ASSERT(strcmp(fileSystemInfo->magic, FILE_SYSTEM_MAGIC) == 0);
    ASSERT(fcbTable != NULL);
    ASSERT(filename != NULL);
    Int32 index = findFileDescriptor(filename);
    if (index != -1)
    {
        if (!fcbTable[index].isOpen)
        {
            return FALSE;
        }
        fcbTable[index].isExists = FALSE;
        freeSector(fcbTable[index].startSector, fcbTable[index].sectorCount);
        (fileSystemInfo->fileDescriptorCount)--;
        writeBackToDisk();
        return TRUE;
    }
    return FALSE;
}

Bool appendToPcb(Pcb *pcb, Uint32 index)
{
    for (Uint32 i = 0; i < index; i++)
    {
        if (pcb->openedFcb[i] == -1)
        {
            pcb->openedFcb[i] = index;
            return TRUE;
        }
    }
    return FALSE;
}

Bool deleteFromPcb(Pcb *pcb, Uint32 index)
{
    for (Uint32 i = 0; i < index; i++)
    {
        if (pcb->openedFcb[i] == index)
        {
            pcb->openedFcb[i] = -1;
            return TRUE;
        }
    }
    return FALSE;
}

void readDisk(Uint32 startSector, Uint32 count, void *buffer)
{
    Byte *buf = (Byte *)buffer;
    for (Uint32 i = 0; i < count; i++)
    {
        _asm_read_disk(startSector + i, buf + i * 512);
    }
}

void writeDisk(Uint32 startSector, Uint32 count, void *buffer)
{
    Byte *buf = (Byte *)buffer;
    for (Uint32 i = 0; i < count; i++)
    {
        _asm_write_disk(startSector + i, buf + i * 512);
    }
}