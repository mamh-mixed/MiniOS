#ifndef __KEYBOARD_H__

#define __KEYBOARD_H__

#define KEYBOARD_BUF_PORT 0x60

#include <stddef.h>
#include <type.h>
#include <queue.h>

/**
 * 用转义字符定义的控制字符.
 */
#define ESC '\033'
#define BACKSPACE '\b'
#define TAB '\t'
#define ENTER '\r'
#define delete '\177'

/**
 * 不可见字符定义为0.
 */
#define CHAR_INVISIBLE 0
#define CTRL_L_CHAR CHAR_INVISIBLE
#define CHAR_R_INVISIBLE CHAR_INVISIBLE
#define SHIFT_L_CHAR CHAR_INVISIBLE
#define SHIFT_R_CHAR CHAR_INVISIBLE
#define ALT_L_CHAR CHAR_INVISIBLE
#define ALT_R_CHAR CHAR_INVISIBLE
#define CAPS_LOCK_CHAR CHAR_INVISIBLE

/**
 * 控制字符的通码和断码.
 */
#define SHIFT_L_MAKE 0x2a
#define SHIFT_R_MAKE 0x36
#define ALT_L_MAKE 0x38
#define ALT_R_MAKE 0xe038
#define ALT_R_BREAK 0xe0b8
#define CTRL_L_MAKE 0x1d
#define CTRL_R_MAKE 0xe01d
#define CTRL_r_BREAK 0xe09d
#define CAPS_LOCK_MAKE 0x3a

void initKeyboardDriver();

void keyboardDriver(Uint32 scanCode);

char getchar();

#endif