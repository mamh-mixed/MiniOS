#include <gdt.h>

static Uint16 codeSelector4GBDpl3, dataSelector4GBDpl3, stackSelector4GBDpl3;
static TaskStateSegment *pTss = NULL;

void initGdtManagement()
{
    GdtDescriptor codeDescriptor = makeGdtDescriptor(0x00000000, 0x000fffff, GDT_DESCRIPTOR_ATTRIBUTE_CODE_4GB_DPL_3);
    GdtDescriptor dataDescriptor = makeGdtDescriptor(0x00000000, 0x000fffff, GDT_DESCRIPTOR_ATTRIBUTE_DATA_4GB_DPL_3);
    GdtDescriptor stackDescriptor = makeGdtDescriptor(0x00000000, 0x000fffff, GDT_DESCRIPTOR_ATTRIBUTE_STACK_4GB_DPL_3);
    codeSelector4GBDpl3 = installGdtDescriptor(codeDescriptor, 3);
    dataSelector4GBDpl3 = installGdtDescriptor(dataDescriptor, 3);
    stackSelector4GBDpl3 = installGdtDescriptor(stackDescriptor, 3);
}

GdtDescriptor makeGdtDescriptor(Uint32 baseAddr, Uint32 limit, Uint32 attribute)
{
    GdtDescriptor descriptor;
    memset(&descriptor, 0, sizeof(GdtDescriptor));
    descriptor.limitLow16 = (Uint16)(limit & 0x0000ffff);
    descriptor.baseAddrLow16 = (Uint16)(baseAddr & 0x0000ffff);
    descriptor.baseAddrMid8 = (Uint8)((baseAddr & 0x00ff0000) >> 16);
    descriptor.baseAddrHight8 = (Uint8)((baseAddr & 0xff000000) >> 24);
    descriptor.attributeLow8 = (Uint8)(attribute & 0x000000ff);
    Uint32 limitHight4 = (Uint32)((limit & 0x000f0000) >> 16);
    Uint32 attributeHight4 = (Uint32)((attribute & 0x00000f00) >> 8);
    descriptor.limithigh4AndAttributeHight4 = (attributeHight4 << 4) | (limitHight4);
    return descriptor;
}

Uint16 installGdtDescriptor(GdtDescriptor descriptor, Uint32 rpl)
{
    GDTR gdtr;
    GDTR *pgdtr = &gdtr;
    memset(pgdtr, 0, sizeof(GDTR));
    asm volatile(
        "sgdt (%%eax)"
        :
        :"a"(pgdtr)
        :
    );

    ASSERT((pgdtr->limit + 1) % sizeof(GdtDescriptor) == 0);
    Uint32 index = (pgdtr->limit + 1) / sizeof(GdtDescriptor);
    memcpy(&pgdtr->table[index], &descriptor, sizeof(descriptor));
    pgdtr->limit = (index + 1) * sizeof(GdtDescriptor) - 1;

    asm volatile(
        "lgdt (%%eax);"
        :
        :"a"(pgdtr)
        :"memory"
    );

    Uint16 selector = 0;
    selector |= rpl;
    selector |= index << 3;
    return selector;
}

Uint16 getGdtSelectorCode4GBDpl3()
{
    return codeSelector4GBDpl3;
}

Uint16 getGdtSelectorData4GBDpl3()
{
    return dataSelector4GBDpl3;
}

Uint16 getGdtSelectorStack4GBDpl3()
{
    return stackSelector4GBDpl3;
}