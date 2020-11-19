#ifndef __GDT_H__
#define __GDT_H__

#include <stddef.h>
#include <type.h>
#include <string.h>

#define GDT_SELECTOR_4GB_CODE_DPL_0 0x10

#define GDT_SELECTOR_4GB_DATA_DPL_0 0x08

#define GDT_SELECTOR_4GB_STACK_DPL_0 0x20

#define GDT_DESCRIPTOR_ATTRIBUTE_CODE_4GB_DPL_3 0x00000cf8
#define GDT_DESCRIPTOR_ATTRIBUTE_DATA_4GB_DPL_3 0x00000cf2
#define GDT_DESCRIPTOR_ATTRIBUTE_STACK_4GB_DPL_3 0x00000cf6
#define GDT_DESCRIPTOR_ATTRIBUTE_TSS_DPL_0 0x00000089

void initGdtManagement();

GdtDescriptor makeGdtDescriptor(Uint32 baseAddr, Uint32 limit, Uint32 attribute);

Uint16 installGdtDescriptor(GdtDescriptor descriptor, Uint32 rpl);

Uint16 getGdtSelectorCode4GBDpl3();

Uint16 getGdtSelectorData4GBDpl3();

Uint16 getGdtSelectorStack4GBDpl3();

#endif