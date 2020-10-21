#ifndef generator_h
#define generator_h
#include <stdbool.h>
#include <stdlib.h>
#include <ucontext.h>
#include <unistd.h>
#include <signal.h>

typedef struct Generator {
    ucontext_t caller_ctx;
    ucontext_t callee_ctx;
    ucontext_t return_ctx;
    char st[MINSIGSTKSZ]; // minimal 32K for Mac; 2K for Linux
    void (*function)(struct Generator*);
    unsigned iterations;
    void* value;
    void* message;
    bool done;
} Generator;

void GeneratorReturn(Generator* this);
Generator* GeneratorInit(Generator* this, void (*function)(Generator*));
Generator* GeneratorMake(void (*function)(Generator*));
void GeneratorFree(Generator* this);
void* GeneratorYield(Generator* this, void* value);
void* GeneratorNext(Generator* this, void* message);

#endif
