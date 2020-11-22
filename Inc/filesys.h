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

// 初始化磁盘扇区位图
void initBitmap();

// 格式化生磁盘
void formatDisk();

// 从硬盘读取出文件系统描述信息
void readFileSystemInfo();

// 将文件系统描述信息写回洗盘
void writeFileSystemInfo();

// 从硬盘中读出所有 Fcb
void readAllFcb();

// 将所有的 Fcb 写回硬盘
void writeAllFcb();

// writeFileSystemInfo & writeAllFcb
void writeBackToDisk();

void createStdin();

void createStdout();

// 分配若干个连续的扇区
Uint32 allocSector(Uint32 sectorCount);

// 释放若干个连续的扇区
void freeSector(Uint32 startSector, Uint32 sectorCount);

// 从 FCB 表中找到指定的 FCB
Int32 findFileDescriptor(const char *filename);

// 从 FCB 表中找到一个空位
Int32 findFreeFileDescriptor();

// 获取所有文件的列表
Uint32 getFileList(char *buffer);

// 获取 FCB 表中指定下标对应的 FCB
Fcb *getFcb(Uint32 index);

// 打开文件，当且仅当 mode == MapOpen 时后两个参数有效。
Bool openFile(const char *filename, OpenMode mode, Uint32 fileOffset, Uint32 mapLength);

Bool closeFile(const char *filename);

Bool createFile(const char *filename, const FileType fileType);

Bool readFile(const char *filename, void *buffer);

Bool writeFile(const char *filename, void *buffer, Uint32 bufferSize);

Bool deleteFile(const char *filename);

// 将 Fcb 添加到 PCB 的已经打开的文件集合中
Bool appendToPcb(Pcb *pcb, Uint32 index);

// 将 FCB 从 PCB 中已经打开的文件集合中删除
Bool deleteFromPcb(Pcb *pcb, Uint32 index);

void readDisk(Uint32 startSector, Uint32 count, void *buffer);

void writeDisk(Uint32 startSector, Uint32 count, void *buffer);

#endif