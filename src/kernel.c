#include <kernel.h>

int main()
{
	_asm_init_8259a();
	initIDT();
	setupIDT();
	interruptEnable();
	initMemoryManagement();

	while (1)
	{
		/* code */
	}
}
