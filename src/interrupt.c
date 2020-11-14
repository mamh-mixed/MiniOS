#include <interrupt.h>

void initIDT()
{
    int idtIndex = 0;
    for (int i = 0; i < 0x28; i++, idtIndex++)
    {
        makeInterruptGateDescriptor(&idt[idtIndex], (_asm_intr_entry_table[i].entry),
                                    INTERRULT_GATE_DESCRIPTOR__ATTRVUTE_DPL_0);
    }

    idtIndex = 0x70;

    for (int i = 0x28; i < 48; i++, idtIndex++)
    {
        makeInterruptGateDescriptor(&idt[idtIndex], (_asm_intr_entry_table[i].entry),
                                    INTERRULT_GATE_DESCRIPTOR__ATTRVUTE_DPL_0);
    }
}

void setupIDT()
{
    pIdt.baseAddr = idt;
    pIdt.limit = 0x78 * 8 - 1;
    asm volatile(
        "lidt (%%eax)"
        :
        : "a"(&pIdt)
        :);
}

void makeInterruptGateDescriptor(InterruptGateDescriptor *descriptor, InterruptGateEntry entry, Uint8 attribute)
{
    Uint16 low16 = (Uint32)(entry)&0x0000ffff;
    Uint16 high16 = ((Uint32)(entry)&0xffff0000) >> 16;
    descriptor->entryLow16 = low16;
    descriptor->entryHight16 = high16;
    descriptor->dcount = 0;
    descriptor->entrySelector = GDT_SELECTOR_DPL_0_4GB_CODE;
    descriptor->attribute = attribute;
}

void interruptDispatcher(Uint32 vevtor, Uint32 errorCode)
{
    char str[50];
    if (vevtor != 0x27 && vevtor != 0x77 && vevtor != 0x20)
    {
        puts(itoa(vevtor, str, 16));
        puts("\n");
    }
}

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
