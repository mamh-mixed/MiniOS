[bits 32]

; 有的中断会压入错误代码，所以就无需压入额外的数据。
%define ERROR_CODE nop

; 有的中断不会压入错误代码，所以压入一个 0 用来维持栈平衡。
%define ZERO push dword 0

; extern __cdecl interruptDispatcher(Uint32 vevtor, Uint32 extra);
; 中断调度器，所有的中断李成均调用此函数以实现真正的功能
extern interruptDispatcher

global _asm_intr_entry_table:

; 中断例程入口表，每项前 200 Byte 为每个中断例程的代码，第 200 ~ 204 Byte 为对应例程的入口。
_asm_intr_entry_table:

; 由于中断例程几乎都一样，所以用一个宏来批量创建。
%macro VECTOR 2
    section .text
        ; 导出中断入口标签
        global _asm_int%1_entry

        ; %1 为宏的第一个参数，表示中断向量号，此处用来形成代表中断入口的标签。
        _asm_int%1_entry:
            %2 ; %2 为宏的第二个参数，如果此中断会压入错误代码则为 nop，反之则压入 0 用来维持栈平衡。

            pushad
            push dword [esp+0x04*8]
            push dword %1 ; 压入中断向量号
            call interruptDispatcher ; 调用中断调度器实现真正的功能
            add esp,8 ; 平衡栈顶

            mov al,0x20 ; 中断结束命令EOI
            out 0xa0,al ;向8259A从片发送
            out 0x20,al ;向8259A主片发送

            popad
            add esp,4

            iret

        times 200-($-_asm_int%1_entry) db 0
; section .data align=1
    dd _asm_int%1_entry

%endmacro


; 下面使用生成 0x00 ~ 0x19 、0x20 ~ 0x27 和 0x70 ~ 0x77 中断
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
; VECTOR 0x20,ZERO

section .text

    ; Bool isProcessChange(); 本次上下文切换是否涉及到切换进程
    extern isProcessChange
    ; Bool isRingChange(); 本次任务切换是否会有特权级变化
    extern isRingChange
    ; void sched(); 找出下一个要运行的任务
    extern sched
    ; Uint32 getNextEsp0(); 获取下一个任务的 esp0
    extern getNextEsp0
    ; Uint32 getNextCr3(); 获取下一个进程的 cr3
    extern getNextCr3
    ; void saveCurEsp0(Uint32 esp0); 保存当前任务的 esp0 到 TCB 中
    extern saveCurEsp0
    ; void switchToNext(Uint32 esp0); 将当前 PCB 和 TCB 设置为即将运行的任务的 PCB 和 TCB，
    ; 并更新 TCB 和 TSS 的 esp0 字段
    extern switchToNext
    global _asm_int0x20_entry:

    _asm_int0x20_entry:

        ; 在当前的内核栈中压入上下文
        push ds
        push es
        push fs
        push gs
        pushad
        
        mov eax,esp
        push eax
        call saveCurEsp0
        add esp,4

        call sched
        call isProcessChange

        cmp eax,0x00 ; 是否涉及到进程切换
        je .b1
        .b0:
            ; 涉及到进程切换
            call getNextCr3
            mov cr3,eax
        .b1:
        call getNextEsp0
        mov [.esp_bak],eax
        call isRingChange
        cmp eax,0x01 ; 特权级是否发生变化
        je .b2
        jmp .b3
        .b2:
            ; 特权级发生了变化
            mov eax,[.esp_bak]
            add eax,68 ; 计算恢复上下文且中断返回后的内核栈栈顶指针
            push eax
            call switchToNext
            add esp,4
            jmp .b4
        .b3:
            ; 特权级未发生变化
            mov eax,[.esp_bak]
            add eax,60 ; 计算恢复上下文且中断返回后的内核栈栈顶指针
            push eax
            call switchToNext ; 切换到下一个 TCB 并设置 TSS 中的 esp0 字段
            add esp,4
        .b4:
        mov esp,[.esp_bak]
        popad
        pop gs
        pop fs
        pop es
        pop ds

        push eax
        mov al,0x20 ; 中断结束命令EOI
        out 0xa0,al ; 向8259A从片发送
        out 0x20,al ; 向8259A主片发送
        pop eax

        iret
    .esp_bak dd 0
    times 200-($-_asm_int0x20_entry) db 0

; section .data align=1
    dd _asm_int0x20_entry

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

        mov al,0x20 ; 中断结束命令EOI
        out 0xa0,al ; 向8259A从片发送
        out 0x20,al ; 向8259A主片发送

        ; mov al,0x0c ; 寄存器C的索引。且开放NMI
        ; out 0x70,al
        ; in al,0x71 ; 读一下RTC的寄存器C，否则只发生一次中断。此处不考虑闹钟和周期性中断的情况。

        pop eax

        iret

        

        ; pesp dd 0

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

    extern getCurRing
    ; Uint32 getCurRing();
    extern syscallDispatcher
    ; Uint32 syscallDispatcher(SysCall syscall, Uint32 argc, Uint32 *esp0);
    global _asm_int0x80_entry:

    _asm_int0x80_entry:

        push ebp
        mov ebp,esp
        push ebx
        push ecx
        push edx
        push esi
        push edi

        mov ebx,[ebp+0x04*3] ; 获取用户栈栈顶指针
        xor esi,esi ; 系统调用参数下标，从右到左。
        xor edi,edi

        cmp ecx,0
        jz .b1
        ; 压入所有参数
        .b0:
            mov edi,esi
            shl edi,2
            add edi,ebx
            push dword [edi]
            loop .b0
        
        .b1:
        mov ecx,esi ; 循环完毕后 esi 就是参数的数量
        mov [_asm_int0x80_ecx_bak],ecx
        mov ebx,esp
        push ebx ; 压入内核栈顶指针
        push ecx ; 压入参数数量
        push eax ; 压入系统调用号
        call syscallDispatcher
        add esp,12
        
        cmp ecx,0
        jz .b3
        mov ecx,[_asm_int0x80_ecx_bak]
        .b2:
            add esp,4
            loop .b2
        
        .b3:
        pop edi
        pop esi
        pop edx
        pop ecx
        pop ebx
        pop ebp

        iret

    _asm_int0x80_ecx_bak dd 0

    times 200-($-_asm_int0x80_entry) db 0
    dd _asm_int0x80_entry



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
