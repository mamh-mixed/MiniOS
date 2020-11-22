#include <sched.h>

static LinkList processList;
static Uint16 tssSelectorDpl0;
static TaskStateSegment *pTss = NULL;
static Pcb *curPcb = NULL, *nextPcb = NULL;
static Tcb *curTcb = NULL, *nextTcb = NULL;

void initScheduler()
{
    linkListNoSyncInit(&processList);
    pTss = (TaskStateSegment *)sysMalloc(sizeof(TaskStateSegment));
    pTss->ss0 = GDT_SELECTOR_4GB_DATA_DPL_0;
    pTss->ioMapOffset = sizeof(TaskStateSegment);
    pTss->ss = getGdtSelectorData4GBDpl3();
    GdtDescriptor tssDescriptor = makeGdtDescriptor((Uint32)pTss, sizeof(TaskStateSegment) - 1, GDT_DESCRIPTOR_ATTRIBUTE_TSS_DPL_0);
    tssSelectorDpl0 = installGdtDescriptor(tssDescriptor, 0);
    asm volatile(
        "ltr %%ax;"
        :
        : "a"(tssSelectorDpl0)
        :);

    Pcb *kernelPcb = (Pcb *)sysMalloc(sizeof(Pcb));
    kernelPcb->id = 0;
    kernelPcb->dpl = 0;
    kernelPcb->status = Ready;
    Uint32 cr3;
    asm volatile(
        "mov %%cr3,%%eax;"
        : "=a"(cr3)::);
    kernelPcb->cr3 = cr3;
    ASSERT(cr3 != 0);
    linkListNoSyncInit(&kernelPcb->threadList);
    Tcb *kernelTcb = createThread(kernelPcb, 0, "kernel", NULL, NULL);
    appendPcb(kernelPcb);
    curPcb = kernelPcb;
    curTcb = kernelTcb;
}

void appendPcb(Pcb *pcb)
{
    InterruptStatus oldStatus = interruptDisable();
    linkListNoSyncPush(&processList, pcb);
    interruptSetStatus(oldStatus);
}

void removePcb(Pcb *pcb)
{
    InterruptStatus oldStatus = interruptDisable();
    ASSERT(1 == 0);
    interruptSetStatus(oldStatus);
}

Bool isProcessChange()
{
    InterruptStatus oldStatus = interruptDisable();
    Bool ret = curPcb == nextPcb ? FALSE : TRUE;
    interruptSetStatus(oldStatus);
    return ret;
}

Bool isRingChange()
{
    InterruptStatus oldStatus = interruptDisable();
    Bool ret = curPcb->dpl == nextPcb->dpl ? FALSE : TRUE;
    interruptSetStatus(oldStatus);
    return ret;
}

void sched()
{
    InterruptStatus oldStatus = interruptDisable();
    LinkListItem *item = NULL;
    Tcb *tcb = NULL;
    Pcb *pcb = NULL;
    if (curPcb->status == Ready)
    {
        item = linkListNoSyncGetNext(&curPcb->threadList);
        if (item == NULL)
        {
            pcb = findNextPcb(&processList);
            tcb = findNextTcb(&pcb->threadList);
        }
        else
        {
            pcb = curPcb;
            tcb = item->data;
        }
    }
    else
    {
        pcb = findNextPcb(&processList);
        tcb = findNextTcb(&pcb->threadList);
    }

    nextTcb = tcb;
    nextPcb = pcb;
    interruptSetStatus(oldStatus);
}

Pcb *findNextPcb(LinkList *list)
{
    LinkListItem *item = NULL;
    while (1)
    {
        item = linkListNoSyncGetNext(list);
        if (item != NULL && ((Pcb *)(item->data))->status == Ready)
        {
            return item->data;
        }
    }
}

Tcb *findNextTcb(LinkList *list)
{
    LinkListItem *item = NULL;
    while (1)
    {
        item = linkListNoSyncGetNext(list);
        if (item != NULL && ((Tcb *)(item->data))->status == Ready)
        {
            return item->data;
        }
    }
}

Uint32 getNextEsp0()
{
    InterruptStatus oldStatus = interruptDisable();
    ASSERT(nextTcb != NULL);
    Uint32 ret = nextTcb->esp0;
    interruptSetStatus(oldStatus);
    return ret;
}

Uint32 getNextCr3()
{
    InterruptStatus oldStatus = interruptDisable();
    ASSERT(nextPcb != NULL);
    ASSERT(nextPcb->cr3 != 0);
    Uint32 ret = nextPcb->cr3;
    interruptSetStatus(oldStatus);
    return ret;
}

void saveCurEsp0(Uint32 esp0)
{
    InterruptStatus oldStatus = interruptDisable();
    ASSERT(curTcb != NULL);
    ASSERT(pTss != NULL);
    curTcb->esp0 = esp0;
    // pTss->esp0 = esp0;
    interruptSetStatus(oldStatus);
}

void switchToNext(Uint32 esp0)
{
    InterruptStatus oldStatus = interruptDisable();
    ASSERT(nextPcb != NULL);
    ASSERT(nextTcb != NULL);
    curPcb = nextPcb;
    curTcb = nextTcb;
    pTss->esp0 = esp0;
    curTcb->esp0 = esp0;
    nextPcb = NULL;
    nextTcb = NULL;
    interruptSetStatus(oldStatus);
}

Pcb *getCurPcb()
{
    InterruptStatus oldStatus = interruptDisable();
    Pcb *ret = curPcb;
    interruptSetStatus(oldStatus);
    return ret;
}

Tcb *getCurTcb()
{
    InterruptStatus oldStatus = interruptDisable();
    Tcb *ret = curTcb;
    interruptSetStatus(oldStatus);
    return ret;
}

Uint32 getCurRing()
{
    InterruptStatus oldStatus = interruptDisable();
    Uint32 ret = curPcb->dpl;
    interruptSetStatus(oldStatus);
    return ret;
}