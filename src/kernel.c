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
	// 	char str[50] = "string";
	// 	queueSyncPush(&queue, str);
	// 	for (int i = 0; i < 9999; i++);
		// {
		// }
		// puts("World   ");
	}
}

int main()
{
	_asm_init_8259a();
	initIDT();
	setupIDT();
	initMemoryManagement();
	initStdioManagement();
	initKeyboardDriver();

	Tcb *tcba = createThread(0, "funca", funca, NULL);
	Tcb *tcbb = createThread(0, "funcb", funcb, NULL);
	curTcb = tcba;
	nextTcb = tcbb;
	startThread(tcba);
	startThread(tcbb);

	interruptEnable();

	/*
		问题在于线程安全的队列会在键盘中断期间进入自旋状态，这样中断就一直无法返回，IF 标志无法置位，整个系统也就崩掉了。
	*/

	while (1)
	{
		// char *str = queueSyncGetFront(&queue);
		// queueSyncPop(&queue);
		// if (str != NULL)
		// {
		// 	puts(str);
		// }
		// puts("World   ");
		char c = getchar();
		if (c != '\0')
		{
			putchar(c);
		}
	}
}
