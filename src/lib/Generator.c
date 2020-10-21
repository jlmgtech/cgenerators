#include <stdio.h>
#include "../include/Generator.h"

void GeneratorReturn(Generator* this) {
    this->done = true;
    ucontext_t caller_ctx = this->caller_ctx;
    // would free the stack here if malloc'd
    setcontext(&caller_ctx);
}

Generator* GeneratorInit(Generator* this, void (*function)(Generator*)) {
    this->function = function;
    this->iterations = 0;
    this->message = NULL;
    this->value = NULL;
    this->done = false;

    // completion context
    getcontext(&this->return_ctx);
    this->return_ctx.uc_stack.ss_sp = this->st;
    this->return_ctx.uc_stack.ss_size = sizeof(this->st);
    this->return_ctx.uc_link = &this->caller_ctx;
    makecontext(&this->return_ctx, (void (*)(void))GeneratorReturn, 1, this);

    // activation context
    getcontext(&this->callee_ctx);
    this->callee_ctx.uc_stack.ss_sp = this->st;
    this->callee_ctx.uc_stack.ss_size = sizeof(this->st);
    this->callee_ctx.uc_link = &this->return_ctx;
    makecontext(&this->callee_ctx, (void (*)(void))this->function, 1, this);

}

Generator* GeneratorMake(void (*function)(Generator*)) {
    Generator* this = malloc(sizeof(Generator));
    GeneratorInit(this, function);
    return this;
}

void GeneratorFree(Generator* this) {
    free(this);
}

void* GeneratorYield(Generator* this, void* value) {
    this->value = value;
    swapcontext(&this->callee_ctx, &this->caller_ctx);
    return this->message;
}

void* GeneratorNext(Generator* this, void* message) {
    this->message = message;
    if (!this->done) {
        this->iterations++;
        if (swapcontext(&this->caller_ctx, &this->callee_ctx)) {
            fprintf(stderr, "ERROR\n");
        }
        return this->value;
    } else {
        return NULL;
    }
}

