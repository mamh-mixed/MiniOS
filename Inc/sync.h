#ifndef SYNC_H

#define SYNC_H 0

#include <stddef.h>
#include <debug.h>
#include <interrupt.h>

typedef struct
{
    Int32 value;
} Semaphore;

void semaphoreInit(Semaphore *semaphore, Int32 value);

void semaphoreGet(Semaphore *semaphore);

void semaphoreRelease(Semaphore *semaphore);

#endif