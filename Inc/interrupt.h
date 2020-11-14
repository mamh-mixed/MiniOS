#ifndef INTERRUPT_H

#define INTERRUPT_H 0

#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <debug.h>

#define EFLAGS_IF 0x00000200 // eflags 寄存器中的 if 位
#define GET_EFLAGS(EFLAG_VAR) asm volatile("pushfl; popl %0" \
                                           : "=g"(EFLAG_VAR))
#define INTERRULT_GATE_DESCRIPTOR__ATTRVUTE_DPL_0 0X8e
#define INTERRULT_GATE_DESCRIPTOR_ATTRVUTE_DPL_3 0XEe

enum InterruptStatus
{
    INTERRUPT_OFF,
    INTERRUPT_ON
};

typedef void *InterruptGateEntry;

#pragma pack(1)
typedef struct
{
    Uint16 entryLow16;
    Uint16 entrySelector;
    Uint8 dcount;

    Uint8 attribute;
    Uint16 entryHight16;

} InterruptGateDescriptor;

typedef struct
{
    Uint16 limit;
    void* baseAddr;
} IDTR;

typedef struct
{
    Uint8 code[200];
    InterruptGateEntry *entry;
} InterruptGateEntryTable;

#pragma pack()

extern InterruptGateEntryTable _asm_intr_entry_table[48];

IDTR pIdt;

InterruptGateDescriptor idt[0x78];

extern void _asm_setup_idt();

extern void _asm_init_8259a();

void initIDT();

void setupIDT();

void makeInterruptGateDescriptor(InterruptGateDescriptor *descriptor, InterruptGateEntry entry, Uint8 attribute);

void interruptDispatcher(Uint32 vevtor, Uint32 errorCode);

enum InterruptStatus interruptGetStatus();

enum InterruptStatus interruptSetStatus(enum InterruptStatus status);

enum InterruptStatus interruptEnable();

enum InterruptStatus interruptDisable();

#endif