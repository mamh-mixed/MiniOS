#include <process.h>

void initProcessManagement()
{
}

Pcb *createProcess(Uint32 id, Uint32 dpl)
{
    Pcb *pcb = (Pcb *)sysMalloc(sizeof(Pcb));
    pcb->id = id;
    pcb->dpl = dpl;
    pcb->status = Ready;
    pcb->cr3 = initAUserPageDir();
    ASSERT(pcb->cr3 != 0);
    // memoryPoolInit(&pcb->memoryPool, sysMalloc(0x8000), 0x00000000, 0x40000, 4096);
    linkListNoSyncInit(&pcb->threadList);
    return pcb;
}

void startProcess(Pcb *pcb)
{
    InterruptStatus oldStatus = interruptDisable();
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