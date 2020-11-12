%include "inc/const.asm"

%define GET_PROGRAM_HEADER_SIZE(base_addr) (base_addr+42)
%define GET_PROGRAM_HEADER_COUNT(base_addr) (base_addr+44)
%define GET_PROGRAM_HEADER_OFFSET(base_addr) (base_addr+28)
%define GET_ENTRY(base_addr) (base_addr+24)


section loader vstart=const_vaddr_loader_entry

    [bits 32]

    jmp start

    start:

        mov eax,const_num_disk_sector_kernel
        mov esi,const_addr_buf
        mov ecx, const_num_disk_sector_kernel_count

        .b0:
            call read_hard_disk
            add esi,512
            inc eax
            loop .b0

        mov eax,const_addr_buf

        xor ebx,ebx
        xor ecx,ecx
        xor edx,edx

        mov bx,[GET_PROGRAM_HEADER_COUNT(eax)]
        mov ecx,[GET_PROGRAM_HEADER_OFFSET(eax)]
        mov dx,[GET_PROGRAM_HEADER_SIZE(eax)]
		mov esi,[GET_ENTRY(eax)]

        add eax,ecx
		add eax,edx ; 跳过 program header 的第一项，因为第一项不包含任何代码和运行时所需的数据。（如果哪天代码出了问题或许和没有加载第一项有关）
        mov [program_header_base_addr],eax
        mov [program_header_each_size],edx
        mov [program_header_num],ebx
		mov [kernel_entry],esi

		cld ; 使用 rep 传送数据时 esi edi 每次传送完递增
		
		; 依次加载 program header 中 LOAD 类型的段（除了第一个段）（因为它的虚拟地址离入口太远，那里没有分配物理页）
        .b1:
            mov ebx,[eax+0x0] ; 取出段类型
            cmp ebx,0x1 ; 判断段类型是否为 PT_LOAD
            jne .b2
            mov esi,[eax+0x04] ; 取出缓冲区内该段的偏移
			add esi,const_addr_buf ; 缓冲区基址 + 段偏移
            mov edi,[eax+0x08] ; 取出程序头表中指定的虚拟地址
            push ecx ; 保存当前循环次数
            mov ecx,[eax+0x10]  ; 取出该段的大小
            rep movsb ; 将缓冲区内的数据传送到置顶的虚拟地址
			pop ecx ; 恢复循环次数
            .b2:
				add eax,[program_header_each_size] ; 移动到下一个 program header 项
                loop .b1

		mov eax,[kernel_entry] ; 取出内核入口
		jmp eax ; 控制权转移到内核





	read_hard_disk:
		;输入 ds  缓冲区段 
		;     esi 缓冲区偏移
		;     eax 扇区号

		push eax
		push ebx
		push ecx
		push esi

		push eax

		mov dx,0x1f2
		mov al,0x01
		out dx,al

		inc dx
		pop eax
		out dx,al

		inc dx
		shr eax,8
		out dx,al

		inc dx
		shr eax,8
		out dx,al

		inc dx
		shr eax,8
		or al,0xe0
		out dx,al

		inc dx
		mov al,0x20
		out dx,al

		.wait_disk:
		in al,dx
		and al,1000_1000b
		cmp al,0000_1000b
		jnz .wait_disk

		mov ecx,256
		mov dx,0x1f0

		.read_disk:
		in ax,dx
		mov [esi],ax
		add esi,2
		loop .read_disk

		pop esi
		pop ecx
		pop ebx
		pop eax
		ret
        




program_header_num dw 0
program_header_base_addr dw 0
program_header_each_size dw 0
kernel_entry dw 0
