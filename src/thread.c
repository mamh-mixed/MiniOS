#include <thread.h>

Tcb *createThread(Uint32 id, const char *name, ThreadFunc func, void *args)
{
    Tcb *tcb = sysMalloc(sizeof(Tcb));
    ASSERT(tcb != NULL);
    tcb->id = id;
    tcb->name = name;
    tcb->cs = GDT_SELECTOR_DPL_0_4GB_CODE;
    tcb->ds = GDT_SELECTOR_DPL_0_4GB_DATA;
    tcb->es = GDT_SELECTOR_DPL_0_4GB_DATA;
    tcb->fs = GDT_SELECTOR_DPL_0_4GB_DATA;
    tcb->gs = GDT_SELECTOR_DPL_0_4GB_DATA;
    tcb->ss0 = GDT_SELECTOR_DPL_0_4GB_STACK;
    tcb->esp0 = (Uint32)sysMalloc(4096) + 4096;
    ASSERT(tcb->esp0 != 0);
    tcb->eip = (Uint32)func;
    return tcb;
}

void startThread(Tcb *tcb)
{
    tcb->esp0 = _asm_start_thread(tcb->cs, tcb->ds, tcb->es, tcb->fs, tcb->gs, tcb->esp0, tcb->eip);
}

Uint32 getNextEsp0()
{
    return nextTcb->esp0;
}

void setCurEsp0(Uint32 esp0)
{
    curTcb->esp0 = esp0;
}

void goNextTcb()
{
    Tcb *temp = curTcb;
    curTcb = nextTcb;
    nextTcb = temp;
}