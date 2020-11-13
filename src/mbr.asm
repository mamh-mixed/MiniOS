%include "inc/const.asm"
		
		
section mbr vstart=0x7c00

	jmp start
		
	pgdt dw 0
		 dd 0x7e00

	
	start:
		mov ax,cs
		mov ds,ax
		mov es,ax
		mov ss,ax
		
		; 取出 GDT 首地址
		mov ebx,[pgdt+0x02]
		
		; 4GB数据段描述符
		mov dword [ebx+0x08],0000_0000_0000_0000_1111_1111_1111_1111b
		mov dword [ebx+0x0c],0000_0000_1100_1111_1001_0010_0000_0000b
		
		; 4GB代码段描述符
		mov dword [ebx+0x10],0000_0000_0000_0000_1111_1111_1111_1111b
		mov dword [ebx+0x14],0000_0000_1100_1111_1001_1000_0000_0000b

		; 显示缓冲区描述符
		mov dword [ebx+0x18],1000_0000_0000_0000_1111_1111_1111_1111b
		mov dword [ebx+0x1c],0000_0000_0100_0000_1001_0010_0000_1011b
		
		; 4GB内核栈段描述符
		mov dword [ebx+0x20],0000_0000_0000_0000_1111_1111_1111_1111b
		mov dword [ebx+0x24],0000_0000_1100_1111_1001_0110_0000_0000b
		
		; 修改GDT界限
		mov word [pgdt],39
		
		; 加载 GDT
		lgdt [pgdt]
		
		; 打开A20
		in al,0x92
		or al,0000_0010b
		out 0x92,al
		
		; 由于中断向量表未设置所以先关中断
		cli
		
		; 进入保护模式
		mov eax,cr0
		or eax,1
		mov cr0,eax
		
		
		jmp dword const_num_gdt_selector_4gb_code:flush
		
		
	[bits 32]
		
		
	flush:
		mov eax,const_num_gdt_selector_4gb_data
		mov ds,eax
		mov es,eax
		mov gs,eax
		mov fs,eax
		mov ss,eax
		mov ss,eax
		mov esp,0x7c00

		mov ecx,1024
		xor edi,edi

		; 清空页目录
		.b0:
			mov dword [const_pvaddr_page_dir_table_start+edi],0x0000_0000
			add edi,4
			loop .b0

		; 创建页目录自己对应的页目录项
		mov edi,const_pvaddr_page_dir_table_start
		mov dword [edi+4092],0x0002_0003

		; 创建 0x0000_0000 ~ 0x003f_ffff 的页目录项，即低端 4MB 内存的页目录项。
		mov dword [edi+0x00],0x0002_1003

		; 创建 0x0040_0000 ~ 0x007f_ffff 的页目录项，即低端 4MB ~ 8MB 的页目录项
		mov dword [edi+0x04],0x0002_2003

		; 清空低端 8MB 内存对应页表
		mov edi,0x0002_1000  ; 0x0002_1000 是页目录项中第 0 项的指向的页表的起始地址
		xor esi,esi   ; esi 对应页表项下标
		mov ecx,2048
		.b1:
			mov dword [edi+esi*0x04],0x0000_0000
			inc esi
			loop .b1
		
		; 创建低端 8MB 内存对应的页表
		xor eax,eax   ; eax 存储起始的物理地址
		xor esi,esi   ; esi 对应页表项下标
		mov ecx,2048

		.b2:
			mov ebx,0x03  ; 页表项属性
			or ebx,eax  ; 形成页表项
			mov [edi+esi*0x4],ebx  ; 写入页表项
			inc esi
			add eax,0x1000  ; 物理地址每次增加 4096KB
			loop .b2

		; 令CR3寄存器指向页目录，并正式开启页功能 
        mov eax,0x00020000                 ;PCD=PWT=0
        mov cr3,eax

        mov eax,cr0
        or eax,0x80000000
        mov cr0,eax                        ;开启分页机制

		; 创建线性地址 0x8000_0000 ~ 0x803f_ffff 对应的页目录项
		mov edi,0xffff_f000  ; 页目录的线性地址
		mov eax,0x8000_0000
		shr eax,22	; 右移 22 位得到高 10 位，即线性地址的高 10 位，即页目录项索引。
		shl eax,2  ; 页目录索引 * 4 得到页目录项在页目录内的偏移
		mov dword [edi+eax],0x0002_1003 ; 将物理内存的低端 4MB 映射到线性地址的 0x8000_0000 ~ 0x800f_ffff

		; 创建线性地址 0x8040_0000 ~ 0x807f_ffff 对应的页目录项
		mov eax,0x8040_0000
		shr eax,22	; 右移 22 位得到高 10 位，即线性地址的高 10 位，即页目录项索引。
		shl eax,2  ; 页目录索引 * 4 得到页目录项在页目录内的偏移
		mov dword [edi+eax],0x0002_2003 ; 将物理内存的低端 4MB ~ 8MB 映射到线性地址的 0x8040_0000 ~ 0x807f_ffff

		sgdt [pgdt]  ; 取出 GDTR 
		mov eax,[pgdt+2]  ; 取出 GDT 基址
		or eax,0x8000_0000  ; 修改为线性地址
		lgdt [pgdt]  ; 写回 GDTR

		mov eax,cosnt_num_disk_sector_loader
		mov esi,const_vaddr_loader_entry
		call read_hard_disk

		jmp const_vaddr_loader_entry


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

	
		times 510-($-$$) db 0
                         db 0x55,0xaa		