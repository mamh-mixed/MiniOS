NASM_FORMAT_BIN=bin
NASM_FORMAT_ELF=elf
OPTIMIZERS_LEVEL=0

define clean
	@rm -rf bin/*.bin obj/*.o
endef

# nasm(format, dest, origin)
define nasm
	@nasm -f $(1) -o $(2)  $(3)
endef

# gcc(-On, dest, origin)
define gcc
	@gcc-10 -O$(1) -m32 -I inc -c -Wno-builtin-declaration-mismatch -Wno-strict-aliasing -fno-stack-protector -finput-charset=UTF-8 -o $(2) $(3)
endef

# ld(entryName, entryAddr, dest, origin)
define ld
	@ld -m elf_i386 -e $(1) -Ttext $(2) -o $(3) $(4)
endef


mbr : src/mbr.asm inc/const.asm Makefile
	$(call nasm,$(NASM_FORMAT_BIN),bin/mbr.bin,src/mbr.asm)

clean : Makefile
	$(call clean)

kernel : src/kernel.c Makefile
	@make util
	$(call gcc,$(OPTIMIZERS_LEVEL),obj/kernel.o,src/kernel.c)
	$(call gcc,$(OPTIMIZERS_LEVEL),obj/string.o,src/string.c)
	$(call gcc,$(OPTIMIZERS_LEVEL),obj/stdlib.o,src/stdlib.c)
	$(call gcc,$(OPTIMIZERS_LEVEL),obj/stdio.o,src/stdio.c)
	$(call gcc,$(OPTIMIZERS_LEVEL),obj/c_interrupt.o,src/interrupt.c)
	$(call gcc,$(OPTIMIZERS_LEVEL),obj/bitmap.o,src/bitmap.c)
	$(call gcc,$(OPTIMIZERS_LEVEL),obj/memory.o,src/memory.c)
	$(call gcc,$(OPTIMIZERS_LEVEL),obj/debug.o,src/debug.c)
	$(call gcc,$(OPTIMIZERS_LEVEL),obj/linklist.o,src/linklist.c)
	$(call gcc,$(OPTIMIZERS_LEVEL),obj/c_thread.o,src/thread.c)
	$(call gcc,$(OPTIMIZERS_LEVEL),obj/sync.o,src/sync.c)
	$(call gcc,$(OPTIMIZERS_LEVEL),obj/queue.o,src/queue.c)
	$(call gcc,$(OPTIMIZERS_LEVEL),obj/keyboard.o,src/keyboard.c)
	$(call gcc,$(OPTIMIZERS_LEVEL),obj/gdt.o,src/gdt.c)
	$(call gcc,$(OPTIMIZERS_LEVEL),obj/c_process.o,src/process.c)
	$(call gcc,$(OPTIMIZERS_LEVEL),obj/sched.o,src/sched.c)
	$(call gcc,$(OPTIMIZERS_LEVEL),obj/syscall.o,src/syscall.c)
	$(call gcc,$(OPTIMIZERS_LEVEL),obj/filesys.o,src/filesys.c)
	$(call nasm,$(NASM_FORMAT_ELF),obj/asm_interrupt.o,src/interrupt.asm)
	$(call nasm,$(NASM_FORMAT_ELF),obj/asm_thread.o,src/thread.asm)
	$(call nasm,$(NASM_FORMAT_ELF),obj/asm_process.o,src/process.asm)
	$(call ld,main,0x80100000,bin/kernel.bin,\
	obj/asm_util.o obj/c_util.o obj/string.o obj/stdlib.o obj/asm_interrupt.o \
	obj/c_interrupt.o obj/debug.o obj/stdio.o obj/memory.o obj/bitmap.o obj/linklist.o \
	obj/asm_thread.o obj/c_thread.o obj/sync.o obj/queue.o obj/keyboard.o \
	obj/gdt.o obj/asm_process.o obj/c_process.o obj/sched.o obj/syscall.o obj/filesys.o  \
	obj/kernel.o)

loader : src/loader.asm inc/const.asm Makefile
	$(call nasm,$(NASM_FORMAT_BIN),bin/loader.bin,src/loader.asm)


util : src/util.asm src/util.c inc/util.h Makefile
	$(call gcc,0,obj/c_util.o,src/util.c)
	$(call nasm,$(NASM_FORMAT_ELF),obj/asm_util.o,src/util.asm)

test : test/fstest.asm test/empty.asm Makefile
	$(call nasm,$(NASM_FORMAT_BIN),bin/fstest.bin,test/fstest.asm)
	$(call nasm,$(NASM_FORMAT_BIN),bin/empty.bin,test/empty.asm)


all : Makefile
	@make clean mbr loader kernel test