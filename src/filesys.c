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
    Uint32 sectorCount =  (ALIGN_512(sizeof(Fcb)) / 512);
    Uint32 index = 0;
    for (; index < fileSystemInfo->fileDescriptorCount; index++)
    {
        readDisk(sector, sectorCount, &fcbTable[index]);
        sector += sectorCount;
    }
    for (; index < fileSystemInfo->fileDescriptorMaxCount; index++)
    {
        fcbTable[index].isExists = FALSE;
    }
}

void writeAllFcb()
{
    ASSERT(fileSystemInfo != NULL);
    ASSERT(strcmp(fileSystemInfo->magic, FILE_SYSTEM_MAGIC) == 0);
    ASSERT(fcbTable != NULL);
    Uint32 sector = fileSystemInfo->fileDescriptorStartSector;
    Uint32 sectorCount =  (ALIGN_512(sizeof(Fcb)) / 512);
    for (Uint32 i = 0; i < fileSystemInfo->fileDescriptorMaxCount; i++)
    {
        if (fcbTable[i].isExists)
        {
            writeDisk(sector, sectorCount, &fcbTable[i]);
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
    fcbTable[1].startSector = File_SYSTEM_FILE_START_SECTOR + 1;
    fcbTable[1].sectorCount = 1;
    strcpy(fcbTable[1].filename, "stdout");
}

Uint32 allocSector(Uint32 sectorCount)
{
    ASSERT(fileSystemInfo != NULL);
    ASSERT(strcmp(fileSystemInfo->magic, FILE_SYSTEM_MAGIC) == 0);
    ASSERT(fcbTable != NULL);
    return bitMapScanAndSet(&sectorMap, sectorCount) + fileSystemInfo->fileStartSector;
}

void freeSector(Uint32 startSector, Uint32 sectorCount)
{
    ASSERT(fileSystemInfo != NULL);
    ASSERT(strcmp(fileSystemInfo->magic, FILE_SYSTEM_MAGIC) == 0);
    ASSERT(fcbTable != NULL);
    Uint32 bitIndex = startSector - fileSystemInfo->fileStartSector;
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

void createFile(const char *filename)
{
    ASSERT(fileSystemInfo != NULL);
    ASSERT(strcmp(fileSystemInfo->magic, FILE_SYSTEM_MAGIC) == 0);
    ASSERT(fcbTable != NULL);
    ASSERT(filename != NULL);
    Int32 index = findFreeFileDescriptor();
    ASSERT(index != -1);
    fcbTable[index].fileType = Gernal;
    fcbTable[index].isExists = TRUE;
    strcpy(fcbTable[index].filename, filename);
    fcbTable[index].startSector = allocSector(1);
    fcbTable[index].sectorCount = 1;
    fcbTable[index].byteSize = 0;
    (fileSystemInfo->fileDescriptorCount)++;
    writeBackToDisk();
}

void readFile(const char *filename, void *buffer)
{
    ASSERT(fileSystemInfo != NULL);
    ASSERT(strcmp(fileSystemInfo->magic, FILE_SYSTEM_MAGIC) == 0);
    ASSERT(fcbTable != NULL);
    ASSERT(filename != NULL);
    ASSERT(buffer != NULL);
    Int32 index = findFileDescriptor(filename);
    ASSERT(index != -1);
    readDisk(fcbTable[index].startSector, fcbTable[index].sectorCount, buffer);
}

void writeFile(const char *filename, void *buffer, Uint32 bufferSize)
{
    ASSERT(fileSystemInfo != NULL);
    ASSERT(strcmp(fileSystemInfo->magic, FILE_SYSTEM_MAGIC) == 0);
    ASSERT(fcbTable != NULL);
    ASSERT(filename != NULL);
    ASSERT(buffer != NULL);
    ASSERT(bufferSize != 0);
    ASSERT(bufferSize % 512 == 0);
    Int32 index = findFileDescriptor(filename);
    ASSERT(index != -1);
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

void deleteFile(const char *filename)
{
    ASSERT(fileSystemInfo != NULL);
    ASSERT(strcmp(fileSystemInfo->magic, FILE_SYSTEM_MAGIC) == 0);
    ASSERT(fcbTable != NULL);
    ASSERT(filename != NULL);
    Int32 index = findFileDescriptor(filename);
    if (index != -1)
    {
        fcbTable[index].isExists = FALSE;
        freeSector(fcbTable[index].startSector, fcbTable[index].sectorCount);
        (fileSystemInfo->fileDescriptorCount)--;
    }
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