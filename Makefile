NASM_FORMAT_BIN=bin
NASM_FORMAT_ELF=elf

define clean
	@rm -rf bin/*.bin obj/*.o
endef

# nasm(format, dest, origin)
define nasm
	@nasm -f $(1) -o $(2)  $(3)
endef

# gcc(-On, dest, origin)
define gcc
	@gcc -O$(1) -m32 -I inc -c -Wno-strict-aliasing -fno-stack-protector -finput-charset=UTF-8 -o $(2) $(3)
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
	$(call gcc,0,obj/kernel.o,src/kernel.c)
	$(call ld,main,0x80100000,bin/kernel.bin,obj/kernel.o)

loader : src/loader.asm inc/const.asm Makefile
	$(call nasm,$(NASM_FORMAT_BIN),bin/loader.bin,src/loader.asm)


all : Makefile
	@make clean mbr loader kernel