#ifndef KERNEL_H

#define KERNEL_h

#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <debug.h>

#define SETUP_INTERRUPT_DESCRIPTOR(idt, index, entry, low32, high32) { \
    (low32) = 0; \
    (high32) = 0; \
    (low32) = (0x10 << 16) | ((entry) & 0x0000ffff); \
    (high32) = ((entry) & 0xffff0000) | (0x8e00); \
    (idt)[(index) * 2] = (low32); \
    (idt)[(index) * 2 + 1] = (high32); \
}


// 最多存储 512 个中断门描述符
extern Uint32 _asm_idt[512 * 2];

extern Uint32 _asm_int0x00_entry;
extern Uint32 _asm_int0x01_entry;
extern Uint32 _asm_int0x02_entry;
extern Uint32 _asm_int0x03_entry;
extern Uint32 _asm_int0x04_entry;
extern Uint32 _asm_int0x05_entry;
extern Uint32 _asm_int0x06_entry;
extern Uint32 _asm_int0x07_entry;
extern Uint32 _asm_int0x08_entry;
extern Uint32 _asm_int0x09_entry;
extern Uint32 _asm_int0x0a_entry;
extern Uint32 _asm_int0x0b_entry;
extern Uint32 _asm_int0x0c_entry;
extern Uint32 _asm_int0x0d_entry;
extern Uint32 _asm_int0x0e_entry;
extern Uint32 _asm_int0x0f_entry;
extern Uint32 _asm_int0x10_entry;
extern Uint32 _asm_int0x11_entry;
extern Uint32 _asm_int0x12_entry;
extern Uint32 _asm_int0x13_entry;
extern Uint32 _asm_int0x14_entry;
extern Uint32 _asm_int0x15_entry;
extern Uint32 _asm_int0x16_entry;
extern Uint32 _asm_int0x17_entry;
extern Uint32 _asm_int0x18_entry;
extern Uint32 _asm_int0x19_entry;
extern Uint32 _asm_int0x1a_entry;
extern Uint32 _asm_int0x1b_entry;
extern Uint32 _asm_int0x1c_entry;
extern Uint32 _asm_int0x1d_entry;
extern Uint32 _asm_int0x1e_entry;
extern Uint32 _asm_int0x1f_entry;
extern Uint32 _asm_int0x20_entry;
extern Uint32 _asm_int0x21_entry;
extern Uint32 _asm_int0x22_entry;
extern Uint32 _asm_int0x23_entry;
extern Uint32 _asm_int0x24_entry;
extern Uint32 _asm_int0x25_entry;
extern Uint32 _asm_int0x26_entry;
extern Uint32 _asm_int0x27_entry;
extern Uint32 _asm_int0x70_entry;
extern Uint32 _asm_int0x71_entry;
extern Uint32 _asm_int0x72_entry;
extern Uint32 _asm_int0x73_entry;
extern Uint32 _asm_int0x74_entry;
extern Uint32 _asm_int0x75_entry;
extern Uint32 _asm_int0x76_entry;
extern Uint32 _asm_int0x77_entry;



extern void _asm_setup_idt();

extern void _asm_init_8259a();

void installIDT();

void interruptDispatcher(Uint32 vevtor, Uint32 errorCode);


#endif