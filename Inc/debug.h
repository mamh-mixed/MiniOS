#ifndef __DEBUG_H__

#define __DEBUG_H__ 0

#include <stddef.h>
#include <type.h>
#include <interrupt.h>

#define PANIC(...) panic_spin (__FILE__, __LINE__, __func__, __VA_ARGS__)

#ifdef NDEBUG
    #define ASSERT(CONDITION) ((void)0)
#else
    #define ASSERT(CONDITION) \
        if(!(CONDITION)){PANIC(#CONDITION);}

void panic_spin(const char* filename, int line, const char* func, const char* condition);

#endif

#endif