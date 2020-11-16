#include <sync.h>

void semaphoreInit(Semaphore *semaphore, Int32 value)
{
    ASSERT(semaphore != NULL);
    ASSERT(value > 0);
    semaphore->value = value;
}

void semaphoreGet(Semaphore *semaphore)
{
    ASSERT(semaphore != NULL);
    InterruptStatus oldStatus = interruptDisable();
    Int32 value = semaphore->value;
    oldStatus = interruptSetStatus(oldStatus);

    while (value <= 0)
    {
        oldStatus = interruptDisable();
        value = semaphore->value;
        oldStatus = interruptSetStatus(oldStatus);
    }

    oldStatus = interruptDisable();
    (semaphore->value)--;
    oldStatus = interruptSetStatus(oldStatus);
}

void semaphoreRelease(Semaphore *semaphore)
{
    ASSERT(semaphore != NULL);
    InterruptStatus oldStatus = interruptDisable();
    (semaphore->value)++;
    oldStatus = interruptSetStatus(oldStatus);
}