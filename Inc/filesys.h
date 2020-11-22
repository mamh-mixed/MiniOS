#ifndef __FILE_SYS_H__

#define __FILE_SYS_H__

#include <stddef.h>
#include <type.h>
#include <string.h>
#include <sched.h>

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

Fcb *getFcb(Uint32 index);

Bool openFile(const char *filename, OpenMode mode, Uint32 fileOffset, Uint32 mapLength);

Bool closeFile(const char *filename);

Uint32 mmap(const char *filename);

Bool munmap(const char *filename);

Bool createFile(const char *filename, const FileType fileType);

Bool readFile(const char *filename, void *buffer);

Bool writeFile(const char *filename, void *buffer, Uint32 bufferSize);

Bool deleteFile(const char *filename);

Bool appendToPcb(Pcb *pcb, Uint32 index);

Bool deleteFromPcb(Pcb *pcb, Uint32 index);

void readDisk(Uint32 startSector, Uint32 count, void *buffer);

void writeDisk(Uint32 startSector, Uint32 count, void *buffer);

#endif