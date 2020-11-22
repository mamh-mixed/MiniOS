#include <kernel.h>

void funca()
{
	while (1)
	{
		// for (int i = 0; i < 999; i++);
		// puts("Hello   ");
	}
}

void funcb()
{
	while (1)
	{

		puts("Hello   ");
		for (int i = 0; i < 99999; i++)
			;
	}
}

int main()
{
	initInterruptManagement();
	initMemoryManagement();
	initStdioManagement();
	initKeyboardDriver();
	initGdtManagement();
	initScheduler();
	initFileSystem();

	Pcb *pcb = createProcess(1024, 3);
	Tcb *tcb = createThread(pcb, 0, "funcb", (ThreadFunc*)0x10000000, NULL);
	startProcess(pcb);

	Uint32 cr3;

	asm volatile(
		"mov %%cr3,%%eax; \
		 mov %%ebx,%%cr3; "
		:"=a"(cr3)
		:"b"(pcb->cr3)
		:"memory"
	);

	_asm_read_disk(100000, (void *)0x10000000);

	asm volatile(
		"mov %0,%%cr3;"
		:
		:"r"(cr3)
		:
	);


	interruptEnable();

	// char str[512];
	// str[1] = 0;
	// ASSERT(openFile("stdin") != FALSE);
	// ASSERT(openFile("stdout") != FALSE);
	Uint32 count = 0;

	while (1)
	{
		puts("Hello   ");
		for (Uint32 i = 0; i < 9999999; i++);
		++count;
		if (count == 10)
		{
			pcb->status = Ready;
			pcb->status = Ready;
			pcb->status = Ready;
			pcb->status = Ready;
		}
		// if (readFile("stdin", str) != FALSE)
		// {
		// 	ASSERT(writeFile("stdout", str, 512) != FALSE);
		// }
	}
}
