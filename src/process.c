#include <process.h>

void initProcessManagement()
{
}

Pcb *createProcess(Uint32 id, Uint32 dpl, Bool isKernelProcess)
{
    Pcb *pcb = (Pcb *)sysMalloc(sizeof(Pcb));
    pcb->id = id;
    pcb->dpl = dpl;
    pcb->status = Suspended;
    if (isKernelProcess)
    {
        asm volatile(
            "mov %%cr3,%%eax;"
            : "=a"(pcb->cr3)::);
    }
    else
    {
        pcb->cr3 = initAUserPageDir();
    }
    ASSERT(pcb->cr3 != 0);
    for (Uint32 i = 0; i < PROCESS_MAX_OPEN; i++)
    {
        pcb->openedFcb[i] = -1;
    }
    if (!isKernelProcess)
    {
        memoryPoolInit(&pcb->memoryPool, sysMalloc(49152), 0x20000000, 393216, 4096); // 0x2000_0000 ~ 0x7fff_ffff
    }
    linkListNoSyncInit(&pcb->threadList);
    return pcb;
}

void startProcess(Pcb *pcb)
{
    InterruptStatus oldStatus = interruptDisable();
    pcb->status = Ready;
    appendPcb(pcb);
    interruptSetStatus(oldStatus);
}

Bool suspendProcess(Pcb *pcb)
{
    pcb->status = Suspended;
    _asm_block_or_suspend_process();
    return TRUE;
}

Bool blockProcess(Pcb *pcb)
{
    pcb->status = Blocked;
    _asm_block_or_suspend_process();
    return TRUE;
}

Bool exitProcess(Pcb *pcb)
{
    pcb->status = Died;
    return TRUE;
}

void destoryProcess(Pcb *pcb)
{
    InterruptStatus oldStatus = interruptDisable();
    freeUserPage((void *)pcb->cr3);
    linkListNoSyncFind(&pcb->threadList, freeKernelStack, NULL);
    sysFree(pcb);
    interruptSetStatus(oldStatus);
}

Bool wakeUpProcess(Pcb *pcb)
{
    pcb->status = Ready;
    return TRUE;
}

Bool freeKernelStack(LinkListItem *item, void *arg)
{
    sysFree((void *)(((Tcb *)(item->data))->esp0));
    return FALSE;
}