#include <thread.h>

Tcb *createThread(Pcb *pcb, Uint32 id, const char *name, void *entry, void *args)
{
    Tcb *tcb = (Tcb *)sysMalloc(sizeof(Tcb));
    ASSERT(tcb != NULL);
    ASSERT(pcb->dpl == 0 || pcb->dpl == 3);
    tcb->id = id;
    tcb->name = name;
    tcb->status = Ready;
    if (pcb->dpl == 0)
    {
        tcb->cs = GDT_SELECTOR_4GB_CODE_DPL_0;
        tcb->ds = GDT_SELECTOR_4GB_DATA_DPL_0;
        tcb->es = GDT_SELECTOR_4GB_DATA_DPL_0;
        tcb->fs = GDT_SELECTOR_4GB_DATA_DPL_0;
        tcb->gs = GDT_SELECTOR_4GB_DATA_DPL_0;
    }
    else
    {
        tcb->cs = getGdtSelectorCode4GBDpl3();
        tcb->ds = getGdtSelectorData4GBDpl3();
        tcb->es = getGdtSelectorData4GBDpl3();
        tcb->fs = getGdtSelectorData4GBDpl3();
        tcb->gs = getGdtSelectorData4GBDpl3();
        tcb->ss3 = getGdtSelectorData4GBDpl3();
    }

    tcb->ss0 = GDT_SELECTOR_4GB_STACK_DPL_0;
    tcb->esp0 = (Uint32)sysMalloc(4096) + 4096;
    tcb->esp3 = 0x2000;
    ASSERT(tcb->esp0 != 0);
    ASSERT(tcb->esp3 != 0);
    tcb->eip = (Uint32)entry;
    if (pcb->dpl == 3)
    {
        tcb->esp0 = _asm_create_thread_dpl3(tcb->cs, tcb->ss3, tcb->ds, tcb->es, tcb->fs, tcb->gs, tcb->esp0, tcb->esp3, tcb->eip);
    }
    else
    {
        tcb->esp0 = _asm_create_thread_dpl0(tcb->cs, tcb->ds, tcb->es, tcb->fs, tcb->gs, tcb->esp0, tcb->eip);
    }
    linkListNoSyncPush(&pcb->threadList, tcb);
    return tcb;
}

void startThread(Tcb *tcb)
{
    tcb->status = Ready;
}

Bool suspendThread(Tcb *tcb)
{
    tcb->status = Suspended;
    _asm_block_or_suspend_thread();
    return TRUE;
}

Bool blockThread(Tcb *tcb)
{
    tcb->status = Blocked;
    _asm_block_or_suspend_thread();
    return TRUE;
}

Bool wakeUpThread(Tcb *tcb)
{
    tcb->status = Ready;
    return TRUE;
}

void exitThread(Tcb *tcb)
{
}