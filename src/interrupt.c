#include <interrupt.h>

enum InterruptStatus interruptGetStatus()
{
    Uint32 eflags = 0;
    GET_EFLAGS(eflags);
    return (EFLAGS_IF & eflags) ? INTERRUPT_ON : INTERRUPT_OFF;
}

enum InterruptStatus interruptSetStatus(enum InterruptStatus status)
{
    return status & INTERRUPT_ON ? interruptEnable() : interruptDisable();
}

enum InterruptStatus interruptEnable()
{
    enum InterruptStatus oldStatus;
    if (INTERRUPT_ON == interruptGetStatus())
    {
        oldStatus = INTERRUPT_ON;
        return oldStatus;
    }

    oldStatus = INTERRUPT_OFF;
    asm volatile("sti");
    return oldStatus;
}

enum InterruptStatus interruptDisable()
{
    enum InterruptStatus oldStatus;
    if (INTERRUPT_OFF == interruptGetStatus())
    {
        oldStatus = INTERRUPT_OFF;
        return oldStatus;
    }

    oldStatus = INTERRUPT_ON;
    asm volatile("cli"
                 :
                 :
                 : "memory");
    return oldStatus;
}
