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

	Pcb *pcb = createProcess(1024, 3, FALSE);
	Tcb *tcb = createThread(pcb, 0, "funcb", (void *)0x10000000, NULL);
	startProcess(pcb);
	startThread(tcb);
	loadProgram(pcb->cr3, 100000, 100, (void *)0x10000000);

	char content[512] = TEXT;
	char filename[] = "README.txt";
	char list[512];

	getFileList(list);

	puts(list);

	if (createFile(filename, GernalFile) == TRUE)
	{
		ASSERT(openFile(filename, GernalOpen, 0, 512) == TRUE);
		ASSERT(writeFile(filename, content, 512) == TRUE);
		ASSERT(closeFile(filename) == TRUE);
	}

	interruptEnable();

	// Uint32 count = 0;

	while (1)
	{
		puts("Hello   ");
		asm volatile("hlt;");
		for (Uint32 i = 0; i < 9999999; i++)
			;
	}
}

void loadProgram(Uint32 cr3, Uint32 startSector, Uint32 sectorCount, void *startAddr)
{
	Uint32 curCr3;
	asm volatile(
		"mov %%cr3,%%eax; \
		 mov %%ebx,%%cr3;"
		: "=a"(curCr3)
		: "b"(cr3)
		: "memory");
	readDisk(startSector, sectorCount, startAddr);
	asm volatile(
		"mov %%eax,%%cr3;" ::"a"(curCr3)
		:);
}
