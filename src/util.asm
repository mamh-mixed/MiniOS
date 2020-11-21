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

global _asm_read_disk

_asm_read_disk:
; extern void _asm_read_disk(Uint32 sector, void *buffer);

    push ebp
    mov ebp,esp
    push eax
    push ecx
    push edx
    push esi

    mov dx,0x1f2 ; 此端口存储要操作的扇区数
    mov al,0x01 ; 仅操作一个扇区
    out dx,al ; 发送到对应端口

    mov eax,[ebp+0x04*2] ; eax 存储上扇区号
    mov esi,[ebp+0x04*3] ; esi 存储缓冲区基址

    mov dx,0x1f3 ; 此端口存储 LBA 扇区号的 0 ~ 7 位
    out dx,al ; 发送 LBA 扇区号的 0 ~ 7 位

    mov dx,0x1f4 ; 此存储 LBA 扇区号的 8 ~ 15 位
    shr eax,8 ; 使 al 为 LBA 扇区号的 8 ~ 15 位
    out dx,al ; 发送 LBA 扇区号的 8 ~ 15 位

    mov dx,0x1f5 ; 此存储 LBA 扇区号的 16 ~ 23 位
    shr eax,8 ; 使 al 为 LBA 扇区号的 16 ~ 23 位
    out dx,al ; 发送 LBA 扇区号的 16 ~ 23 位

    mov dx,0x1f6 ; 此端口存储 LBA 扇区号的 24 ~ 27 位以及一些控制信息
    shr eax,8 ; 使 al 的低 4 位为 LBA 扇区号的 24 ~ 27 位
    and al,0x0f ; 清零 al 的高 4 位
    or al,0xe0 ; 写入控制信息，操作主盘，LBA 模式。
    out dx,al ; 发送 LBA 扇区号的 24 ~ 27 位以及一些控制信息

    mov dx,0x1f7 ; 此端口写时用来控制硬盘行为
    mov al,0x20 ; 硬盘读命令
    out dx,al ; 发送命令到硬盘

    mov dx,0x1f7 ; 此端口在读时表示硬盘的状态
    .wait_disk:
        in al,dx ; 读取硬盘状态
        and al,1000_1000b ; 保留硬盘忙位和数据准备位
        cmp al,0000_1000b ; 判断是否不忙且已经准备好数据
        jnz .wait_disk

    mov dx,0x1f0 ; 此端口读硬盘时代表读取出来的数据，特殊的在于它是 16 位端口。
    mov ecx,256 ; 一次读取 16 位，即 2 Byte，256 次就是 512 Byte。
    .read_disk:
        in ax,dx ; 读取 16 位的数据
        mov [esi],ax ; 写入缓冲区
        add esi,2 ; 给缓冲区基址加偏移
        loop .read_disk

    pop esi
    pop edx
    pop ecx
    pop eax
    pop ebp

	ret

global _asm_write_disk

_asm_write_disk:
; extern void _asm_write_disk(Uint32 sector, void *buffer);

    push ebp
    mov ebp,esp
    push eax
    push ecx
    push edx
    push esi

    mov dx,0x1f2 ; 此端口存储要操作的扇区数
    mov al,0x01 ; 仅操作一个扇区
    out dx,al ; 发送到对应端口

    mov eax,[ebp+0x04*2] ; eax 存储上扇区号
    mov esi,[ebp+0x04*3] ; esi 存储缓冲区基址

    mov dx,0x1f3 ; 此端口存储 LBA 扇区号的 0 ~ 7 位
    out dx,al ; 发送 LBA 扇区号的 0 ~ 7 位

    mov dx,0x1f4 ; 此存储 LBA 扇区号的 8 ~ 15 位
    shr eax,8 ; 使 al 为 LBA 扇区号的 8 ~ 15 位
    out dx,al ; 发送 LBA 扇区号的 8 ~ 15 位

    mov dx,0x1f5 ; 此存储 LBA 扇区号的 16 ~ 23 位
    shr eax,8 ; 使 al 为 LBA 扇区号的 16 ~ 23 位
    out dx,al ; 发送 LBA 扇区号的 16 ~ 23 位

    mov dx,0x1f6 ; 此端口存储 LBA 扇区号的 24 ~ 27 位以及一些控制信息
    shr eax,8 ; 使 al 的低 4 位为 LBA 扇区号的 24 ~ 27 位
    and al,0x0f ; 清零 al 的高 4 位
    or al,0xe0 ; 写入控制信息，操作主盘，LBA 模式。
    out dx,al ; 发送 LBA 扇区号的 24 ~ 27 位以及一些控制信息

    mov dx,0x1f7 ; 此端口写时用来控制硬盘行为
    mov al,0x30 ; 硬盘写命令
    out dx,al ; 发送命令到硬盘

    mov dx,0x1f7 ; 此端口在读时表示硬盘的状态
    .wait_disk:
        in al,dx ; 读取硬盘状态
        and al,1000_1000b ; 保留硬盘忙位和数据准备位
        cmp al,0000_1000b ; 判断是否不忙且已经准备好写数据
        jnz .wait_disk

    mov dx,0x1f0 ; 此端口读硬盘时代表读取出来的数据，特殊的在于它是 16 位端口。
    mov ecx,256 ; 一次读取 16 位，即 2 Byte，256 次就是 512 Byte。
    .write_disk:
        mov ax,[esi] ; 取出要写入的数据
        out dx,ax ; 发送给硬盘
        add esi,2 ; 给缓冲区基址加偏移
        loop .write_disk

    pop esi
    pop edx
    pop ecx
    pop eax
    pop ebp

	ret