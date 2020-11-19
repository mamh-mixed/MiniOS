#ifndef __PROCESS_H__

#define __PROCESS_H__ 0

#include <stddef.h>
#include <type.h>
#include <memory.h>
#include <interrupt.h>
#include <linklist.h>
#include <gdt.h>
#include <sched.h>

void initProcessManagement();

Pcb *createProcess(Uint32 id, Uint32 dpl);

void startProcess(Pcb *pcb);

#endif