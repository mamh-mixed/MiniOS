#include <process.h>

void initProcessManagement()
{
}

Pcb *createProcess(Uint32 id, Uint32 dpl)
{
    Pcb *pcb = (Pcb *)sysMalloc(sizeof(Pcb));
    pcb->id = id;
    pcb->dpl = dpl;
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