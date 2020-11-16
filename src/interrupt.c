#include <interrupt.h>

extern InterruptGateEntryTable _asm_intr_entry_table[48];

IDTR pIdt;

InterruptGateDescriptor idt[0x78];

void initIDT()
{
    // 根据 src/interrupt.asm 的内存布局，中断向量 0x00 ~ 0x27 的入口在 _asm_intr_entry_table[0x0...0x27] 中
    int idtIndex = 0;
    for (int i = 0; i < 0x28; i++, idtIndex++)
    {
        makeInterruptGateDescriptor(&idt[idtIndex], (_asm_intr_entry_table[i].entry),
                                    INTERRULT_GATE_DESCRIPTOR__ATTRVUTE_DPL_0);
    }

    // 根据 src/interrupt.asm 的内存布局，中断向量 0x70 ~ 0x77 的入口在 _asm_intr_entry_table[0x28...48] 中
    idtIndex = 0x70;

    for (int i = 0x28; i < 48; i++, idtIndex++)
    {
        makeInterruptGateDescriptor(&idt[idtIndex], (_asm_intr_entry_table[i].entry),
                                    INTERRULT_GATE_DESCRIPTOR__ATTRVUTE_DPL_0);
    }
}

void setupIDT()
{
    pIdt.baseAddr = idt; // 获取 IDT 基址
    pIdt.limit = 0x78 * 8 - 1; // 计算 IDT 界限，一共有 0x78 个索引，但并不是全部有效，有效的只有 0x00 ~ 0x27 和 0x70 ~ 0x77。
    asm volatile(
        "lidt (%%eax)"
        :
        : "a"(&pIdt)
        :"memory"
    );
}

void makeInterruptGateDescriptor(InterruptGateDescriptor *descriptor, InterruptGateEntry entry, Uint8 attribute)
{
    Uint16 low16 = (Uint32)(entry)&0x0000ffff;
    Uint16 high16 = ((Uint32)(entry)&0xffff0000) >> 16;
    descriptor->entryLow16 = low16;
    descriptor->entryHight16 = high16;
    descriptor->dcount = 0; // 这项永远为 0
    descriptor->entrySelector = GDT_SELECTOR_DPL_0_4GB_CODE;
    descriptor->attribute = attribute;
}

void interruptDispatcher(Uint32 vector, Uint32 errorCode)
{
    char str[50];
    if (vector >= 0 && vector <= 0x13)
    {
        puts(itoa(vector, str, 16));
        puts("\n");
        asm volatile("hlt;");
    }
}

InterruptStatus interruptGetStatus()
{
    Uint32 eflags = 0;
    GET_EFLAGS(eflags);
    return (EFLAGS_IF & eflags) ? INTERRUPT_ON : INTERRUPT_OFF;
}

InterruptStatus interruptSetStatus(InterruptStatus status)
{
    return status & INTERRUPT_ON ? interruptEnable() : interruptDisable();
}

InterruptStatus interruptEnable()
{
    InterruptStatus oldStatus;
    if (INTERRUPT_ON == interruptGetStatus())
    {
        oldStatus = INTERRUPT_ON;
        return oldStatus;
    }

    oldStatus = INTERRUPT_OFF;
    asm volatile("sti");
    return oldStatus;
}

InterruptStatus interruptDisable()
{
    InterruptStatus oldStatus;
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
