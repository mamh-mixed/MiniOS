#ifndef INTERRUPT_H

#define INTERRUPT_H 0

#define EFLAGS_IF 0x00000200 // eflags 寄存器中的 if 位为 
#define GET_EFLAGS(EFLAG_VAR) asm volatile("pushfl; popl %0" : "=g" (EFLAG_VAR))

#include <stddef.h>

enum InterruptStatus
{
    INTERRUPT_OFF, INTERRUPT_ON
};

enum InterruptStatus interruptGetStatus();

enum InterruptStatus interruptSetStatus(enum InterruptStatus status);

enum InterruptStatus interruptEnable();

enum InterruptStatus interruptDisable();


#endif