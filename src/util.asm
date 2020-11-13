[bits 32]

global _asm_get_cursor

_asm_get_cursor:
    ; 获取 VGA 显卡模式下光标的位置
    ; extern __cdecl uint32 _asm_get_cursor();

    push ebx
    push edx

    mov dx,0x3d4 ; 显卡索引寄存器端口号
    mov al,0x0e ; 光标高 8 位寄存器索引
    out dx,al ; 向索引寄存器发送寄存器索引
    mov dx,0x3d5; 显卡寄存器读写取端口
    in al,dx ; 读取光标高 8 位
    
    mov bh,al

    mov dx,0x3d4 ; 显卡索引寄存器端口号
    mov al,0x0f ; 光标低 8 位寄存器索引
    out dx,al ; 向索引寄存器发送寄存器索引
    mov dx,0x3d5; 显卡寄存器读写取端口
    in al,dx ; 读取光标低 8 位

    mov bl,al

    xor eax,eax
    mov ax,bx
    
    and eax,0x0000_ffff ; 清空高 16 位

    pop edx
    pop ebx
    ret


global _asm_set_cursor

_asm_set_cursor:
    ; 设置 VGA 显卡模式下光标的为之
    ; extern __cdecl void _asm_set_cursor(uint16 pos)

    push ebp
    mov ebp,esp
    push ebx
    push edx

    mov ebx,[ebp+8] ; 取出函数参数，即要设置的光标位置。

    mov dx,0x3d4 ; 显卡索引寄存器端口号
    mov al,0x0e ; 光标高 8 位寄存器索引
    out dx,al ; 向索引寄存器发送寄存器索引
    mov dx,0x3d5; 显卡寄存器读写取端口
    mov al,bh ; 获取参数的高 8 位
    out dx,al ; 设置光标的高 8 位

    mov dx,0x3d4 ; 显卡索引寄存器端口号
    mov al,0x0f ; 光标低 8 位寄存器索引
    out dx,al ; 向索引寄存器发送寄存器索引
    mov dx,0x3d5; 显卡寄存器读写取端口
    mov al,bl ; 获取光标的低 8 位
    out dx,al ; 设置光标的低 8 位

    pop edx
    pop ebx
    pop ebp
    ret

