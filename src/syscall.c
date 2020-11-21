#include <syscall.h>

Uint32 syscallDispatcher(SysCall syscall, Uint32 argc, Uint32 *esp0)
{
    switch (syscall)
    {
    case GetPid:
        return syscallGetPid();
        break;
    default:
        ASSERT(1 == 0);
        break;
    }
    return 0;
}

Uint32 syscallGetPid()
{
    return getCurPcb()->id;
}