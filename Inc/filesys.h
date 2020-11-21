#ifndef __FILE_SYS_H__

#define __FILE_SYS_H__

#include <stddef.h>
#include <type.h>
#include <string.h>

#define FILE_SYSTEM_INFO_SECTOR 5000

#define FILE_SYSTEM_MAGIC ("MiniOS_FS")

#define FILE_SYSTEM_FILE_DESCRIPTOR_START_SECTOR 5100

#define File_SYSTEM_FILE_START_SECTOR 10000

#define FILE_SYSTEM_FILE_SECTOR_COUNT 80000

#define FILE_SYSTEM_MAX_FILE_COUNT 5000

#define ALIGN_512(var) (((var) % 512 == 0) ? ((var)) : ((((var) / 512) * 512) + 512))

extern void _asm_read_disk(Uint32 sector, void *buffer);

extern void _asm_write_disk(Uint32 sector, void *buffer);

void initFileSystem();

void initBitmap();

void formatDisk();

void readFileSystemInfo();

void writeFileSystemInfo();

void readAllFcb();

void writeAllFcb();

void writeBackToDisk();

void createStdin();

void createStdout();

Uint32 allocSector(Uint32 sectorCount);

void freeSector(Uint32 startSector, Uint32 sectorCount);

Int32 findFileDescriptor(const char *filename);

Int32 findFreeFileDescriptor();

Uint32 getFileList(char *buffer);

void createFile(const char *filename);

void readFile(const char *filename, void *buffer);

void writeFile(const char *filename, void *buffer, Uint32 bufferSize);

void deleteFile(const char *filename);

void readDisk(Uint32 startSector, Uint32 count, void *buffer);

void writeDisk(Uint32 startSector, Uint32 count, void *buffer);

#endif