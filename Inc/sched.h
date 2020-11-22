#ifndef __SCHED_H__

#define __SCHED_H__

#include <stddef.h>
#include <type.h>
#include <linklist.h>
#include <process.h>
#include <thread.h>

void initScheduler();

void appendPcb(Pcb *pcb);

void removePcb(Pcb *pcb);

Bool isProcessChange();

Bool isRingChange();

void sched();

Uint32 getNextEsp0();

Uint32 getNextCr3();

Pcb *findNextPcb(LinkList *list);

Tcb *findNextTcb(LinkList *list);

void saveCurEsp0(Uint32 esp0);

void switchToNext(Uint32 esp0);

Pcb *getCurPcb();

Tcb *getCurTcb();

Uint32 getCurRing();

#endif