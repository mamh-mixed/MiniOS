#ifndef __SYNC_H__

#define __SYNC_H__ 0

#include <stddef.h>
#include <type.h>
#include <debug.h>
#include <interrupt.h>



void semaphoreInit(Semaphore *semaphore, Int32 value);

void semaphoreGet(Semaphore *semaphore);

void semaphoreRelease(Semaphore *semaphore);

#endif