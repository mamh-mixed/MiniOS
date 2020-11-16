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
		for (int i = 0; i < 99999; i++);
		puts("World   ");
	}
}

int main()
{
	_asm_init_8259a();
	initIDT();
	setupIDT();
	initMemoryManagement();
	initStdout();

	Tcb *tcba = createThread(0, "funca", funca, NULL);
	Tcb *tcbb = createThread(0, "funcb", funcb, NULL);
	curTcb = tcba;
	nextTcb = tcbb;
	startThread(tcba);
	startThread(tcbb);

	interruptEnable();

	while (1)
	{
		for (int i = 0; i < 99999; i++);
		puts("Hello   ");
	}
}
