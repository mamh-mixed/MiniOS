#ifndef __SYSCALL_H__

#define __SYSCALL_H__

#include <stddef.h>
#include <type.h>
#include <sched.h>

/*
    系统调用约定：
        * 中断号：0x80
        * 系统调用号存放于 eax 中
        * 参数个数存放于 ecx 中
        * 参数从右到左依次压栈
        * 压栈完毕后立刻手动引发 0x80 中断
        * 如果有返回值的话则存放在 eax 中
*/

Uint32 syscallDispatcher(SysCall syscall, Uint32 argc, Uint32 *esp0);

Uint32 syscallGetPid();

Int32 syscallOpen(const char* filename);

Int32 syscallRead(const char* filename, void *buffer);

Int32 syscallWrite(const char* filename, void *buffer);

#endif