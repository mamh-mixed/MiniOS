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

void sched();

Uint32 getNextEsp0();

Uint32 getNextCr3();

void saveCurEsp0(Uint32 esp0);

void switchToNext();

#endif