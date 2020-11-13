#ifndef ASM_FUN_H

#define ASM_FUN_H 0

#include <type.h>

// 获取光标的位置
extern unsigned int _asm_get_cursor();

// 设置光标的位置
extern void _asm_set_cursor(Uint16 pos);

#endif