#ifndef THEAD_H

#define THEAD_H 0

#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <memory.h>
#include <debug.h>
#include <interrupt.h>

typedef void(ThreadFunc)();

typedef struct 
{
    Uint32 id;
    const char* name;
    void* args;
    Uint32 eip;
    Uint32 ss0;
    Uint32 esp0;
    Uint32 ss3;
    Uint32 esp3;
    Uint32 cs;
    Uint32 ds;
    Uint32 fs;
    Uint32 es;
    Uint32 gs;
} Tcb;

Tcb *curTcb;

Tcb *nextTcb;

Tcb* createThread(Uint32 id, const char* name, ThreadFunc func, void* args);

extern Uint32 _asm_start_thread(Uint32 cs, Uint32 ds, Uint32 es, Uint32 fs, Uint32 gs, Uint32 esp0, Uint32 eip);

void startThread(Tcb* tcb);

Uint32 getNextEsp0();

void setCurEsp0();

void goNextTcb();

void _asm_test();

#endif