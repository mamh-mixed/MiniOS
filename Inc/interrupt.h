#ifndef __INTERRUPT_H__

#define __INTERRUPT_H__ 0

#include <stddef.h>
#include <type.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <debug.h>
#include <keyboard.h>
#include <linklist.h>
#include <gdt.h>
#include <sched.h>
#include <filesys.h>

#define EFLAGS_IF 0x00000200 // eflags 寄存器中的 if 位
#define GET_EFLAGS(EFLAG_VAR) asm volatile("pushfl; popl %0" \
                                           : "=g"(EFLAG_VAR))
#define INTERRULT_GATE_DESCRIPTOR_ATTRIBUTE_DPL_0 0X8e
#define INTERRULT_GATE_DESCRIPTOR_ATTRIBUTE_DPL_3 0XEe


extern void _asm_setup_idt();

extern void _asm_init_8259a();

void initInterruptManagement();

void initIDT();

void setupIDT();

void makeInterruptGateDescriptor(InterruptGateDescriptor *descriptor, InterruptGateEntry entry, Uint8 attribute);

void interruptDispatcher(Uint32 vector, Uint32 extra);

void pageFaultHandler();

InterruptStatus interruptGetStatus();

InterruptStatus interruptSetStatus(InterruptStatus status);

InterruptStatus interruptEnable();

InterruptStatus interruptDisable();



#endif