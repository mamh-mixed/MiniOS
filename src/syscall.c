#include <syscall.h>

Uint32 syscallDispatcher(SysCall syscall, Uint32 argc, Uint32 *esp0)
{
    switch (syscall)
    {
    case GetPid:
        return syscallGetPid();
    case Open:
        return syscallOpen((char *)esp0[0], (OpenMode)esp0[1], esp0[2], esp0[3]);
    case Close:
        return syscallClose((char *)esp0[0]);
    case Create:
        return syscallCreate((char *)esp0[0], esp0[1]);
    case Read:
        return syscallRead((char *)esp0[0], (void *)esp0[1]);
    case Write:
        return syscallWrite((char *)esp0[0], (void *)esp0[1], esp0[2]);
    case Delete:
        return syscallDelete((char *)esp0[0]);
    case SuspendProcess:
        return syscallSuspendProcess();
    case BlockProcess:
        return syscallBlockedProcess();
    case ExitProcess:
        syscallExitProcess();
        break;
    case SuspendThread:
        return syscallSuspendThread();
    case BlockThread:
        return syscallBlockeddThread();
    case ExitThread:
        syscallExitThread();
        break;
    default:
        ASSERT(1 == 0);
        break;
    }
    return TRUE;
}

Uint32 syscallGetPid()
{
    return getCurPcb()->id;
}

Bool syscallOpen(const char *filename, OpenMode mode, Uint32 fileOffset, Uint32 mapLength)
{
    return openFile(filename, mode, fileOffset, mapLength);
}

Bool syscallClose(const char *filename)
{
    return closeFile(filename);
}

Bool syscallCreate(const char *filename, FileType fileType)
{
    return createFile(filename, fileType);
}

Bool syscallRead(const char *filename, void *buffer)
{
    return readFile(filename, buffer);
}

Bool syscallWrite(const char *filename, void *buffer, Uint32 bufferSize)
{
    return writeFile(filename, buffer, bufferSize);
}

Bool syscallDelete(const char *filename)
{
    return deleteFile(filename);
}

Bool syscallSuspendProcess()
{
    return suspendProcess(getCurPcb());
}

Bool syscallBlockedProcess()
{
    return blockProcess(getCurPcb());
}

void syscallExitProcess()
{
    exitProcess(getCurPcb());
}

Bool syscallSuspendThread()
{
    return suspendThread(getCurTcb());
}

Bool syscallBlockeddThread()
{
    return blockThread(getCurTcb());
}

void syscallExitThread()
{
    exitThread(getCurTcb());
}