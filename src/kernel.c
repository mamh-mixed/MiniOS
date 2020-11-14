#include <kernel.h>

int main()
{
	_asm_init_8259a();
	installIDT();
	int i = 0;
	ASSERT(i == 1);
	while (1)
	{
		/* code */
	}
}

void interruptDispatcher(Uint32 vevtor, Uint32 errorCode)
{
	char str[50];
	if (vevtor != 0x20 && vevtor != 0x70)
	{
		puts(itoa(vevtor, str, 16));
		puts("\n");
	}
}

void installIDT()
{
	Uint32 low32, high32;
	Int32 index = 0;

	SETUP_INTERRUPT_DESCRIPTOR(_asm_idt, index, (Uint32)(&_asm_int0x00_entry), low32, high32);
	index++;

	SETUP_INTERRUPT_DESCRIPTOR(_asm_idt, index, (Uint32)(&_asm_int0x01_entry), low32, high32);
	index++;

	SETUP_INTERRUPT_DESCRIPTOR(_asm_idt, index, (Uint32)(&_asm_int0x02_entry), low32, high32);
	index++;

	SETUP_INTERRUPT_DESCRIPTOR(_asm_idt, index, (Uint32)(&_asm_int0x03_entry), low32, high32);
	index++;

	SETUP_INTERRUPT_DESCRIPTOR(_asm_idt, index, (Uint32)(&_asm_int0x04_entry), low32, high32);
	index++;

	SETUP_INTERRUPT_DESCRIPTOR(_asm_idt, index, (Uint32)(&_asm_int0x05_entry), low32, high32);
	index++;

	SETUP_INTERRUPT_DESCRIPTOR(_asm_idt, index, (Uint32)(&_asm_int0x06_entry), low32, high32);
	index++;

	SETUP_INTERRUPT_DESCRIPTOR(_asm_idt, index, (Uint32)(&_asm_int0x07_entry), low32, high32);
	index++;

	SETUP_INTERRUPT_DESCRIPTOR(_asm_idt, index, (Uint32)(&_asm_int0x08_entry), low32, high32);
	index++;

	SETUP_INTERRUPT_DESCRIPTOR(_asm_idt, index, (Uint32)(&_asm_int0x09_entry), low32, high32);
	index++;

	SETUP_INTERRUPT_DESCRIPTOR(_asm_idt, index, (Uint32)(&_asm_int0x0a_entry), low32, high32);
	index++;

	SETUP_INTERRUPT_DESCRIPTOR(_asm_idt, index, (Uint32)(&_asm_int0x0b_entry), low32, high32);
	index++;

	SETUP_INTERRUPT_DESCRIPTOR(_asm_idt, index, (Uint32)(&_asm_int0x0c_entry), low32, high32);
	index++;

	SETUP_INTERRUPT_DESCRIPTOR(_asm_idt, index, (Uint32)(&_asm_int0x0d_entry), low32, high32);
	index++;

	SETUP_INTERRUPT_DESCRIPTOR(_asm_idt, index, (Uint32)(&_asm_int0x0e_entry), low32, high32);
	index++;

	SETUP_INTERRUPT_DESCRIPTOR(_asm_idt, index, (Uint32)(&_asm_int0x0f_entry), low32, high32);
	index++;

	SETUP_INTERRUPT_DESCRIPTOR(_asm_idt, index, (Uint32)(&_asm_int0x10_entry), low32, high32);
	index++;

	SETUP_INTERRUPT_DESCRIPTOR(_asm_idt, index, (Uint32)(&_asm_int0x11_entry), low32, high32);
	index++;

	SETUP_INTERRUPT_DESCRIPTOR(_asm_idt, index, (Uint32)(&_asm_int0x12_entry), low32, high32);
	index++;

	SETUP_INTERRUPT_DESCRIPTOR(_asm_idt, index, (Uint32)(&_asm_int0x13_entry), low32, high32);
	index++;

	SETUP_INTERRUPT_DESCRIPTOR(_asm_idt, index, (Uint32)(&_asm_int0x14_entry), low32, high32);
	index++;

	SETUP_INTERRUPT_DESCRIPTOR(_asm_idt, index, (Uint32)(&_asm_int0x15_entry), low32, high32);
	index++;

	SETUP_INTERRUPT_DESCRIPTOR(_asm_idt, index, (Uint32)(&_asm_int0x16_entry), low32, high32);
	index++;

	SETUP_INTERRUPT_DESCRIPTOR(_asm_idt, index, (Uint32)(&_asm_int0x17_entry), low32, high32);
	index++;

	SETUP_INTERRUPT_DESCRIPTOR(_asm_idt, index, (Uint32)(&_asm_int0x18_entry), low32, high32);
	index++;

	SETUP_INTERRUPT_DESCRIPTOR(_asm_idt, index, (Uint32)(&_asm_int0x19_entry), low32, high32);
	index++;

	SETUP_INTERRUPT_DESCRIPTOR(_asm_idt, index, (Uint32)(&_asm_int0x1a_entry), low32, high32);
	index++;

	SETUP_INTERRUPT_DESCRIPTOR(_asm_idt, index, (Uint32)(&_asm_int0x1b_entry), low32, high32);
	index++;

	SETUP_INTERRUPT_DESCRIPTOR(_asm_idt, index, (Uint32)(&_asm_int0x1c_entry), low32, high32);
	index++;

	SETUP_INTERRUPT_DESCRIPTOR(_asm_idt, index, (Uint32)(&_asm_int0x1d_entry), low32, high32);
	index++;

	SETUP_INTERRUPT_DESCRIPTOR(_asm_idt, index, (Uint32)(&_asm_int0x1e_entry), low32, high32);
	index++;

	SETUP_INTERRUPT_DESCRIPTOR(_asm_idt, index, (Uint32)(&_asm_int0x1f_entry), low32, high32);
	index++;

	SETUP_INTERRUPT_DESCRIPTOR(_asm_idt, index, (Uint32)(&_asm_int0x20_entry), low32, high32);
	index++;

	SETUP_INTERRUPT_DESCRIPTOR(_asm_idt, index, (Uint32)(&_asm_int0x21_entry), low32, high32);
	index++;

	SETUP_INTERRUPT_DESCRIPTOR(_asm_idt, index, (Uint32)(&_asm_int0x22_entry), low32, high32);
	index++;

	SETUP_INTERRUPT_DESCRIPTOR(_asm_idt, index, (Uint32)(&_asm_int0x23_entry), low32, high32);
	index++;

	SETUP_INTERRUPT_DESCRIPTOR(_asm_idt, index, (Uint32)(&_asm_int0x24_entry), low32, high32);
	index++;

	SETUP_INTERRUPT_DESCRIPTOR(_asm_idt, index, (Uint32)(&_asm_int0x25_entry), low32, high32);
	index++;

	SETUP_INTERRUPT_DESCRIPTOR(_asm_idt, index, (Uint32)(&_asm_int0x26_entry), low32, high32);
	index++;

	SETUP_INTERRUPT_DESCRIPTOR(_asm_idt, index, (Uint32)(&_asm_int0x27_entry), low32, high32);
	index++;

	index = 0x70;

	SETUP_INTERRUPT_DESCRIPTOR(_asm_idt, index, (Uint32)(&_asm_int0x70_entry), low32, high32);
	index++;

	SETUP_INTERRUPT_DESCRIPTOR(_asm_idt, index, (Uint32)(&_asm_int0x71_entry), low32, high32);
	index++;

	SETUP_INTERRUPT_DESCRIPTOR(_asm_idt, index, (Uint32)(&_asm_int0x72_entry), low32, high32);
	index++;

	SETUP_INTERRUPT_DESCRIPTOR(_asm_idt, index, (Uint32)(&_asm_int0x73_entry), low32, high32);
	index++;

	SETUP_INTERRUPT_DESCRIPTOR(_asm_idt, index, (Uint32)(&_asm_int0x74_entry), low32, high32);
	index++;

	SETUP_INTERRUPT_DESCRIPTOR(_asm_idt, index, (Uint32)(&_asm_int0x75_entry), low32, high32);
	index++;

	SETUP_INTERRUPT_DESCRIPTOR(_asm_idt, index, (Uint32)(&_asm_int0x76_entry), low32, high32);
	index++;

	SETUP_INTERRUPT_DESCRIPTOR(_asm_idt, index, (Uint32)(&_asm_int0x77_entry), low32, high32);
	index++;

	_asm_setup_idt();

	__asm__("sti;");
}
