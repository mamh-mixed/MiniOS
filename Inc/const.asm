%ifndef CONST_ASM

%define CONST_ASM 0

; 内核起始扇区号
const_num_disk_sector_kernel equ 2

; 内核代码所占扇区数量
const_num_disk_sector_kernel_count equ 4096

; 内核加载器起始扇区号
cosnt_num_disk_sector_loader equ 1

; VGA 显卡模式下显存首地址
const_pvaddr_vga_mode_start equ 0x000b_8000

; VGA 显卡模式下显存大小
const_num_vag_mode_addr_size equ 0x0000_ffff

; 4GB 数据段 GDT 选择子
const_num_gdt_selector_4gb_data equ 0x08

; 4GB 代码段 GDT 选择子
const_num_gdt_selector_4gb_code equ 0x10

; VGA 显卡模式显存 GDT 选择子
const_num_gdt_selector_vga equ 0x18

; 4GB 栈段 GDT 选择子
const_num_gdt_selector_4gb_kernel_stack equ 0x20

; 页目录物理基址地
const_pvaddr_page_dir_table_start equ 0x20000

; 内核加载器的入口地址（线性地址/虚拟地址）
const_vaddr_loader_entry equ 0x8003_0000

; 加载内核时所用的缓冲区
const_addr_buf equ 0x8004_0000

; 缓冲区大小
const_addr_buf_size equ 0x0020_0000

; elf 文件头中记录程序头表在文件中偏移的偏移
const_num_elf_program_header_offset_in_file equ 0x34

%endif


