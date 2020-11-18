#include <keyboard.h>

/**
 * 记录以下按键是否已被按下.
 */

/*
    之所以不用线性安全的队列，是因为键盘中断会调用键盘驱动向队列中添加击键信息，
    如果获取队列锁的时候被阻塞，此时键盘中断未返回，IF 标志无法置位，也就无法
    响应时钟中断，导致整个系统在锁内循环。
*/
static Bool ctrlStatus, shiftStatus, altStatus, capsLockStatus, extScanCode;
static Queue keyboardQueue;

/**
 * 以通码为索引的显示字符数组，零号元素为shift没有按下时的展示，1反之.
 */
static char keymap[][2] = {
    {0, 0},
    {ESC, ESC},
    {'1', '!'},
    {'2', '@'},
    {'3', '#'},
    {'4', '$'},
    {'5', '%'},
    {'6', '^'},
    {'7', '&'},
    {'8', '*'},
    {'9', '('},
    {'0', ')'},
    {'-', '_'},
    {'=', '+'},
    {BACKSPACE, BACKSPACE},
    {TAB, TAB},
    {'q', 'Q'},
    {'w', 'W'},
    {'e', 'E'},
    {'r', 'R'},
    {'t', 'T'},
    {'y', 'Y'},
    {'u', 'U'},
    {'i', 'I'},
    {'o', 'O'},
    {'p', 'P'},
    {'[', '{'},
    {']', '}'},
    {ENTER, ENTER},
    {CTRL_L_CHAR, CTRL_L_CHAR},
    {'a', 'A'},
    {'s', 'S'},
    {'d', 'D'},
    {'f', 'F'},
    {'g', 'G'},
    {'h', 'H'},
    {'j', 'J'},
    {'k', 'K'},
    {'l', 'L'},
    {';', ':'},
    {'\'', '"'},
    {'`', '~'},
    {SHIFT_L_CHAR, SHIFT_L_CHAR},
    {'\\', '|'},
    {'z', 'Z'},
    {'x', 'X'},
    {'c', 'C'},
    {'v', 'V'},
    {'b', 'B'},
    {'n', 'N'},
    {'m', 'M'},
    {',', '<'},
    {'.', '>'},
    {'/', '?'},
    {SHIFT_R_CHAR, SHIFT_R_CHAR},
    {'*', '*'},
    {ALT_L_CHAR, ALT_L_CHAR},
    {' ', ' '},
    {CAPS_LOCK_CHAR, CAPS_LOCK_CHAR}};

void initKeyboardDriver()
{
    queueNoSyncInit(&keyboardQueue, 256);
}

void keyboardDriver(Uint32 scanCode)
{
    if (scanCode == 0xe0)
    {
        // 扩展字符，等待第二个扫描码
        extScanCode = TRUE;
        return;
    }

    if (extScanCode)
    {
        scanCode = ((0xe000) | scanCode);
        extScanCode = FALSE;
    }

    // 是否是断码
    int isBreakCode = ((scanCode & 0x0080) != 0);

    if (isBreakCode)
    {
        // 获得其通码
        Uint16 makeCode = (scanCode &= 0xff7f);
        if (makeCode == CTRL_L_MAKE || makeCode == CTRL_R_MAKE)
        {
            ctrlStatus = FALSE;
        }
        else if (makeCode == SHIFT_L_MAKE || makeCode == SHIFT_R_MAKE)
        {
            shiftStatus = FALSE;
        }
        else if (makeCode == ALT_L_MAKE || makeCode == ALT_R_MAKE)
        {
            altStatus = FALSE;
        }
        return;
    }

    // 到这里就说明是通码，但是我们只能处理0x01-0x3a之间的按键，再加上两个特殊通码
    if ((scanCode <= 0x00 || scanCode > 0x3a) && (scanCode != ALT_R_MAKE && scanCode != CTRL_R_MAKE))
    {
        // put_str("Unkown key.\n");
        return;
    }

    Bool shift = FALSE;

    if (scanCode < 0x0e || scanCode == 0x29 || scanCode == 0x1a || scanCode == 0x1b || scanCode == 0x2b || scanCode == 0x27 || scanCode == 0x28 || scanCode == 0x33 || scanCode == 0x34 || scanCode == 0x35)
    {
        // 0-9, [等字符不关心caps
        shift = shiftStatus;
    }
    else
    {
        if (shiftStatus && capsLockStatus)
        {
            // shift和caps同时按下，相互抵消
            shift = FALSE;
        }
        else if (shiftStatus || capsLockStatus)
        {
            shift = TRUE;
        }
        else
        {
            shift = FALSE;
        }
    }

    Uint32 index = (scanCode & 0x00ff);

    char curChar = keymap[index][shift];

    if (curChar && !queueNoSyncIsFull(&keyboardQueue))
    {
        char *value = (char *)sysMalloc(sizeof(char));
        *value = curChar;
        queueNoSyncPush(&keyboardQueue, value);
        return;
    }

    if (scanCode == CTRL_L_MAKE || scanCode == CTRL_R_MAKE)
    {
        ctrlStatus = TRUE;
    }
    else if (scanCode == SHIFT_L_MAKE || scanCode == SHIFT_R_MAKE)
    {
        shiftStatus = TRUE;
    }
    else if (scanCode == ALT_R_MAKE || scanCode == ALT_R_MAKE)
    {
        altStatus = TRUE;
    }
    else if (scanCode == CAPS_LOCK_MAKE)
    {
        capsLockStatus = capsLockStatus == TRUE ? FALSE : TRUE;
    }
}

char getchar()
{
    InterruptStatus oldStatus = interruptDisable(); 
    char *value = queueNoSyncGetFront(&keyboardQueue);
    queueNoSyncPop(&keyboardQueue);
    char ret = '\0';
    if (value != NULL)
    {
        ret = *value;
        sysFree(value);
    }
    interruptSetStatus(oldStatus);
    return ret;
}