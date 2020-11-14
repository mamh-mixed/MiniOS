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

		; 填充页目录的第 0 ~ 16 项，对应线性地址的 0x0000_0000 ~ 0x03ff_ffff，共 64MB。
		mov eax,0x0002_1003 ; eax 存储页目录项描述符
		xor esi,esi ; esi 存储页目录索引
		mov ecx,0x10 ; 循环 16 次，每次设置一个页目录项，每个页目录项对应 4MB 地址，共 64MB 地址。
		.b1:
			mov [edi+esi*0x04],eax ; 写入页目录
			add eax,0x1000 ; 计算新的页表地址
			inc esi
			loop .b1

		; 清空低端 64MB 内存对应页表
		mov edi,0x0002_1000  ; 0x0002_1000 是页目录项中第 0 项的指向的页表的起始地址
		xor esi,esi   ; esi 对应页表项下标
		mov ecx,0x4000
		.b2:
			mov dword [edi+esi*0x04],0x0000_0000
			inc esi
			loop .b2
		
		; 创建低端 64MB 内存对应的页表
		xor eax,eax   ; eax 存储起始的物理地址
		xor esi,esi   ; esi 对应页表项下标
		mov ecx,0x4000

		.b3:
			mov ebx,0x03  ; 页表项属性
			or ebx,eax  ; 形成页表项
			mov [edi+esi*0x4],ebx  ; 写入页表项
			inc esi
			add eax,0x1000  ; 物理地址每次增加 4096KB
			loop .b3

		; 令CR3寄存器指向页目录，并正式开启页功能 
        mov eax,0x00020000                 ;PCD=PWT=0
        mov cr3,eax

        mov eax,cr0
        or eax,0x80000000
        mov cr0,eax                        ;开启分页机制

		; 创建线性地址 0x8000_0000 ~ 0x83f_ffff 对应的页目录项，共 64MB。
		mov edi,0xffff_f000  ; 页目录的线性地址

		mov eax,0x8000_0000 ; eax 存储线性地址
		mov ebx,0x0002_1003 ; ebx 存储页表项描述符
		mov ecx,0x10 ; 循环 16 次，每次设置 4MB 的地址，共 64MB。
		.b4:
			mov edx,eax
			shr edx,22	; 右移 22 位得到高 10 位，即线性地址的高 10 位，即页目录项索引。
			shl edx,2  ; 页目录索引 * 4 得到页目录项在页目录内的偏移
			mov dword [edi+edx],ebx ; 写入页表
			add eax,0x0040_0000 ; 线性地址增加 4MB
			add ebx,0x1000 ; 页表描述符中的物理基址增加 4KB
			loop .b4

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