#ifndef __THEAD_H__

#define __THEAD_H__ 0

#include <stddef.h>
#include <type.h>
#include <stdlib.h>
#include <string.h>
#include <memory.h>
#include <debug.h>
#include <gdt.h>
#include <interrupt.h>

Tcb *createThread(Pcb *pcb, Uint32 id, const char *name, void *entry, void *args);

extern Uint32 _asm_create_thread_dpl0(Uint32 cs, Uint32 ds, Uint32 es, Uint32 fs, Uint32 gs, Uint32 esp0, Uint32 eip);

extern Uint32 _asm_create_thread_dpl3(Uint32 cs, Uint32 ss3, Uint32 ds, Uint32 es, Uint32 fs, Uint32 gs, Uint32 esp0, Uint32 esp3, Uint32 eip);

extern void _asm_block_or_suspend_thread();

void startThread(Tcb *tcb);

Bool suspendThread(Tcb *tcb);

Bool blockThread(Tcb *tcb);

Bool wakeUpThread(Tcb *tcb);

void exitThread(Tcb *tcb);

void _asm_test();

#endif