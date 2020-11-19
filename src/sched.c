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
    pTss->ss0 = GDT_SELECTOR_4GB_STACK_DPL_0;
    pTss->ioMapOffset = sizeof(TaskStateSegment);
    pTss->ss = getGdtSelectorStack4GBDpl3();
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
    Uint32 cr3;
    asm volatile(
        "mov %%cr3,%%eax;"
        :"=a"(cr3)::
    );
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
    linkListNoSyncPush(&processList, pcb);
}

void removePcb(Pcb *pcb)
{
    ASSERT(1 == 0);
}

Bool isProcessChange()
{
    return curPcb == nextPcb ? FALSE : TRUE;
}

void sched()
{
    LinkListItem *item = linkListNoSyncGetNext(&curPcb->threadList);
    Tcb *tcb = NULL;
    Pcb *pcb = NULL;
    if (item == NULL)
    {
        item = linkListNoSyncGetNext(&processList);
        item = item == NULL ? linkListNoSyncGetNext(&processList) : item;
        ASSERT(item != NULL);
        pcb = item->data;
        item = linkListNoSyncGetNext(&pcb->threadList);
        item = item == NULL ? linkListNoSyncGetNext(&pcb->threadList) : item;
        ASSERT(item != NULL);
        tcb = item->data;
    } else
    {
        tcb = item->data;
        pcb = curPcb;
    }
    nextTcb = tcb;
    nextPcb = pcb;
    
}

Uint32 getNextEsp0()
{
    ASSERT(nextTcb != NULL);
    return nextTcb->esp0;
}

Uint32 getNextCr3()
{
    ASSERT(nextPcb != NULL);
    ASSERT(nextPcb->cr3 != 0);
    return nextPcb->cr3;
}

void saveCurEsp0(Uint32 esp0)
{
    ASSERT(curTcb != NULL);
    ASSERT(pTss != NULL);
    curTcb->esp0 = esp0;
    pTss->esp0 = esp0;
}

void switchToNext()
{
    ASSERT(nextPcb != NULL);
    ASSERT(nextTcb != NULL);
    curPcb = nextPcb;
    curTcb = nextTcb;
    nextPcb = NULL;
    nextTcb = NULL;
}