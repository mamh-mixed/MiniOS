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
		for (int i = 0; i < 99999; i++);
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

	Pcb *pcb = createProcess(1, 0);
	ASSERT(pcb->cr3 != 0);
	Tcb *tcb = createThread(pcb, 0, "funcb", funcb, NULL);
	startProcess(pcb);


	interruptEnable();

	while (1)
	{
		puts("World   ");
		for (int i = 0; i < 99999; i++);
	}
}
