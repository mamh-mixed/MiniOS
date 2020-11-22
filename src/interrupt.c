#include <interrupt.h>

extern InterruptGateEntryTable _asm_intr_entry_table[49];

static IDTR pIdt;

static InterruptGateDescriptor idt[0x81];

void initInterruptManagement()
{
    _asm_init_8259a();
    initIDT();
    setupIDT();
}

void initIDT()
{
    // 根据 src/interrupt.asm 的内存布局，中断向量 0x00 ~ 0x27 的入口在 _asm_intr_entry_table[0x0...0x27] 中
    int idtIndex = 0;
    for (int i = 0; i < 0x28; i++, idtIndex++)
    {
        makeInterruptGateDescriptor(&idt[idtIndex], (_asm_intr_entry_table[i].entry),
                                    INTERRULT_GATE_DESCRIPTOR_ATTRIBUTE_DPL_0);
    }

    // 根据 src/interrupt.asm 的内存布局，中断向量 0x70 ~ 0x77 的入口在 _asm_intr_entry_table[0x28...48] 中
    idtIndex = 0x70;

    for (int i = 0x28; i < 48; i++, idtIndex++)
    {
        makeInterruptGateDescriptor(&idt[idtIndex], (_asm_intr_entry_table[i].entry),
                                    INTERRULT_GATE_DESCRIPTOR_ATTRIBUTE_DPL_0);
    }

    idtIndex = 0x80;
    makeInterruptGateDescriptor(&idt[idtIndex], (_asm_intr_entry_table[48].entry),
                                INTERRULT_GATE_DESCRIPTOR_ATTRIBUTE_DPL_3);
}

void setupIDT()
{
    pIdt.baseAddr = idt;       // 获取 IDT 基址
    pIdt.limit = 0x81 * 8 - 1; // 计算 IDT 界限，一共有 0x81 个索引，但并不是全部有效，有效的只有 0x00 ~ 0x27 和 0x70 ~ 0x77 和 0x80。
    asm volatile(
        "lidt (%%eax)"
        :
        : "a"(&pIdt)
        : "memory");
}

void makeInterruptGateDescriptor(InterruptGateDescriptor *descriptor, InterruptGateEntry entry, Uint8 attribute)
{
    Uint16 low16 = (Uint32)(entry)&0x0000ffff;
    Uint16 high16 = ((Uint32)(entry)&0xffff0000) >> 16;
    descriptor->entryLow16 = low16;
    descriptor->entryHight16 = high16;
    descriptor->dcount = 0; // 这项永远为 0
    descriptor->entrySelector = GDT_SELECTOR_4GB_CODE_DPL_0;
    descriptor->attribute = attribute;
}

void interruptDispatcher(Uint32 vector, Uint32 extra)
{
    char str[50];
    if (vector >= 0 && vector <= 0x13 && vector != 0x0e && vector != 0x01)
    {
        putUHex(vector);
        puts("\n");
        asm volatile("hlt;");
    }
    else if (vector == 0x0e)
    {
        pageFaultHandler();
    }
    else if (vector == 0x21)
    {
        Uint32 scanCode;
        asm volatile(
            "in $0x60,%%al; \
             xor %%ebx,%%ebx; \
             mov %%al,%%bl;"
            : "=b"(scanCode)
            :
            : "eax");
        keyboardDriver(scanCode);
    }
}

void pageFaultHandler()
{
    Uint32 cr3, cr2;
    asm volatile(
        "mov %%cr3,%0; \
             mov %%cr2,%1;"
        : "=r"(cr3), "=r"(cr2)
        :
        : "memory");
    Pcb *curPcb = getCurPcb();
    Fcb *mapedFcb = NULL;
    for (Uint32 i = 0; i < PROCESS_MAX_OPEN; i++)
    {
        if (curPcb->openedFcb[i] != -1)
        {
            Fcb *fcb = getFcb(curPcb->openedFcb[i]);
            if (fcb->isMaped)
            {
                mapedFcb = fcb;
                break;
            }
        }
    }
    if (mapedFcb != NULL)
    {
        Uint32 mapRangeLeft = (Uint32)(mapedFcb->mapAddr);
        Uint32 mapRangeRight = ((Uint32)(mapedFcb->mapAddr)) + mapedFcb->mapLength;
        Uint32 startSector = mapedFcb->startSector + mapedFcb->fileOffset / 512;
        if (cr2 >= mapRangeLeft && cr2 <= mapRangeRight)
        {
            Uint32 startAddr = (Uint32)(mapedFcb->mapAddr);
            Uint32 endAddr = startAddr + mapedFcb->mapLength;

            for (; startAddr < endAddr; startAddr += 4096)
            {
                installA4KBPage((void *)curPcb->cr3, startAddr, 1, 1);
            }
            readDisk(startSector, mapedFcb->mapLength / 512, mapedFcb->mapAddr);
            return;
        }
    }

    Uint32 us = cr2 >= 0x80000000 ? 0 : 1;
    installA4KBPage((void *)cr3, cr2, 1, us);
}

InterruptStatus interruptGetStatus()
{
    Uint32 eflags = 0;
    GET_EFLAGS(eflags);
    return (EFLAGS_IF & eflags) ? INTERRUPT_ON : INTERRUPT_OFF;
}

InterruptStatus interruptSetStatus(InterruptStatus status)
{
    return status == INTERRUPT_ON ? interruptEnable() : interruptDisable();
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
    asm volatile("sti" ::
                     : "memory");
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
    asm volatile("cli" ::
                     : "memory");
    return oldStatus;
}
