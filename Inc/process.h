#ifndef __PROCESS_H__

#define __PROCESS_H__ 0

#include <stddef.h>
#include <type.h>
#include <memory.h>
#include <interrupt.h>
#include <linklist.h>
#include <gdt.h>
#include <sched.h>

void _asm_process_exit(Uint32 nextCR3, Uint32 nextEsp0);

void _asm_block_or_suspend_process();

void initProcessManagement();

Pcb *createProcess(Uint32 id, Uint32 dpl);

void startProcess(Pcb *pcb);

Bool suspendProcess(Pcb *pcb);

Bool blockProcess(Pcb *pcb);

Bool wakeUpProcess(Pcb *pcb);

Bool exitProcess(Pcb *pcb);

void destoryProcess(Pcb *pcb);

Bool freeKernelStack(LinkListItem *item, void *arg);

#endif