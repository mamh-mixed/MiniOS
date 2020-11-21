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

	// Pcb *pcb = createProcess(1024, 3);
	// Tcb *tcb = createThread(pcb, 0, "funcb", (ThreadFunc*)0x10000000, NULL);
	// startProcess(pcb);

	// Uint32 cr3;

	// asm volatile(
	// 	"mov %%cr3,%%eax; \
	// 	 mov %%ebx,%%cr3; "
	// 	:"=a"(cr3)
	// 	:"b"(pcb->cr3)
	// 	:"memory"
	// );

	// _asm_read_disk(5000, (void *)0x10000000);

	// asm volatile(
	// 	"mov %0,%%cr3;"
	// 	:
	// 	:"r"(cr3)
	// 	:
	// );


	interruptEnable();

	char files[1024];
	getFileList(files);
	puts(files);

	char filename[] = "README.txt";
	char content[512 * 4];

	// createFile(filename);

	// getFileList(files);
	// puts(files);

	
	// memset(content, 0, 512 * 4);
	// strcpy(content, "I am a boy, my name is Hollow Knight!");

	// writeFile(filename, content, 512 * 4);

	// getFileList(files);
	// puts(files);

	memset(content, 0, 512 * 4);
	readFile(filename, content);

	puts(content);

	while (1)
	{
		// puts("Hello   ");
		// asm volatile("hlt;");
	}
}
