[bits 32]

; 有的中断会压入错误代码，所以就无需压入额外的数据。
%define ERROR_CODE nop

; 有的中断不会压入错误代码，所以压入一个 0 用来维持栈平衡。
%define ZERO push dword 0

; extern __cdecl interruptDispatcher(Uint32 vevtor, Uint32 errorCode);
; 中断调度器，所有的中断李成均调用此函数以实现真正的功能
extern interruptDispatcher

global _asm_intr_entry_table:

; 中断例程入口表，每项前 148 Byte 为每个中断例程的代码，第 148 ~ 150 Byte 为对应例程的入口。
_asm_intr_entry_table:

; 由于中断例程几乎都一样，所以用一个宏来批量创建。
%macro VECTOR 2
    section .text
        ; 导出中断入口标签
        global _asm_int%1_entry:

        ; %1 为宏的第一个参数，表示中断向量号，此处用来形成代表中断入口的标签。
        _asm_int%1_entry:
            
            push eax

            
            %2 ; %2 为宏的第二个参数，如果此中断会压入错误代码则为 nop，反之则压入 0 用来维持栈平衡。
            push dword %1 ; 压入中断向量号
            call interruptDispatcher ; 调用中断调度器实现真正的功能
            add esp,8 ; 平衡栈顶

            mov al,0x20 ; 中断结束命令EOI
            out 0xa0,al ;向8259A从片发送
            out 0x20,al ;向8259A主片发送

            pop eax

            iret
        times 200-($-_asm_int%1_entry) db 0
; section .data align=1
    dd _asm_int%1_entry

%endmacro


; 下面使用生成 0x00 ~ 0x19 、0x20 ~ 0x27 和 0x71 ~ 0x77 中断，0x70 号为实时时钟中断，需要单独编写。
VECTOR 0x00,ZERO
VECTOR 0x01,ZERO
VECTOR 0x02,ZERO
VECTOR 0x03,ZERO
VECTOR 0x04,ZERO
VECTOR 0x05,ZERO
VECTOR 0x06,ZERO
VECTOR 0x07,ZERO
VECTOR 0x08,ERROR_CODE
VECTOR 0x09,ZERO
VECTOR 0x0a,ERROR_CODE
VECTOR 0x0b,ERROR_CODE
VECTOR 0x0c,ERROR_CODE
VECTOR 0x0d,ERROR_CODE
VECTOR 0x0e,ERROR_CODE
VECTOR 0x0f,ZERO
VECTOR 0x10,ZERO
VECTOR 0x11,ERROR_CODE
VECTOR 0x12,ZERO
VECTOR 0x13,ZERO
VECTOR 0x14,ZERO
VECTOR 0x15,ZERO
VECTOR 0x16,ZERO
VECTOR 0x17,ZERO
VECTOR 0x18,ZERO
VECTOR 0x19,ZERO
VECTOR 0x1a,ZERO
VECTOR 0x1b,ZERO
VECTOR 0x1c,ZERO
VECTOR 0x1d,ZERO
VECTOR 0x1e,ZERO
VECTOR 0x1f,ZERO
VECTOR 0x20,ZERO
VECTOR 0x21,ZERO
VECTOR 0x22,ZERO
VECTOR 0x23,ZERO
VECTOR 0x24,ZERO
VECTOR 0x25,ZERO
VECTOR 0x26,ZERO
VECTOR 0x27,ZERO


section .text

   global _asm_int0x70_entry:

    _asm_int0x70_entry:
            
        push eax

        push dword 0x00 ; 压入无用数据用来维持栈平衡
        push dword 0x70 ; 压入中断向量号
        call interruptDispatcher
        add esp,8

        mov al,0x20 ; 中断结束命令EOI
        out 0xa0,al ; 向8259A从片发送
        out 0x20,al ; 向8259A主片发送

        mov al,0x0c ; 寄存器C的索引。且开放NMI
        out 0x70,al
        in al,0x71 ; 读一下RTC的寄存器C，否则只发生一次中断。此处不考虑闹钟和周期性中断的情况。

        pop eax

        iret

    times 200-($-_asm_int0x70_entry) db 0

; section .data align=1
    dd _asm_int0x70_entry

VECTOR 0x71,ZERO
VECTOR 0x72,ZERO
VECTOR 0x73,ZERO
VECTOR 0x74,ZERO
VECTOR 0x75,ZERO
VECTOR 0x76,ZERO
VECTOR 0x77,ZERO



section .text
    global _asm_init_8259a

    _asm_init_8259a:

        push eax

    	;设置8259A中断控制器
        mov al,0x11
        out 0x20,al                        ;ICW1：边沿触发/级联方式
        mov al,0x20
        out 0x21,al                        ;ICW2:起始中断向量
        mov al,0x04
        out 0x21,al                        ;ICW3:从片级联到IR2
        mov al,0x01
        out 0x21,al                        ;ICW4:非总线缓冲，全嵌套，正常EOI

        mov al,0x11
        out 0xa0,al                        ;ICW1：边沿触发/级联方式
        mov al,0x70
        out 0xa1,al                        ;ICW2:起始中断向量
        mov al,0x04
        out 0xa1,al                        ;ICW3:从片级联到IR2
        mov al,0x01
        out 0xa1,al                        ;ICW4:非总线缓冲，全嵌套，正常EOI

        ;设置和时钟中断相关的硬件 
        mov al,0x0b                        ;RTC寄存器B
        or al,0x80                         ;阻断NMI
        out 0x70,al
        mov al,0x12                        ;设置寄存器B，禁止周期性中断，开放更
        out 0x71,al                        ;新结束后中断，BCD码，24小时制

        in al,0xa1                         ;读8259从片的IMR寄存器
        and al,0xfe                        ;清除bit 0(此位连接RTC)
        out 0xa1,al                        ;写回此寄存器

        mov al,0x0c
        out 0x70,al
        in al,0x71                         ;读RTC寄存器C，复位未决的中断状态
		
        pop eax
		ret
