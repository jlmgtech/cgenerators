#include <stdio.h>
#include <stdlib.h>
#include "../include/Generator.h"

void GeneratorReturn(Generator* this, void* value) {
    this->done = true;
    this->value = value;
    setcontext(&this->caller_ctx);
    //swapcontext(&this->callee_ctx, &this->caller_ctx);
}

void GeneratorReturnError() {
    // TODO - maybe put some more helpful information here, idk
    fprintf(stderr, "You didn't put a GeneratorReturn!\n");
    exit(1);
}

Generator* GeneratorMake(void (*function)(Generator*)) {

    Generator* this = malloc(sizeof(Generator));

    this->function = function;
    this->iterations = 0;
    this->message = NULL;
    this->value = NULL;
    this->done = false;

    // completion context
    getcontext(&this->return_ctx);
    this->return_ctx.uc_stack.ss_sp = this->closeStack;
    this->return_ctx.uc_stack.ss_size = sizeof(this->closeStack);
    this->return_ctx.uc_link = &this->caller_ctx;
    //makecontext(&this->return_ctx, (void (*)(void))GeneratorReturnError, 0);
    makecontext(&this->return_ctx, (void (*)(void))GeneratorReturn, 2, this, NULL);

    // activation context
    getcontext(&this->callee_ctx);
    this->callee_ctx.uc_stack.ss_sp = this->openStack;
    this->callee_ctx.uc_stack.ss_size = sizeof(this->openStack);
    this->callee_ctx.uc_link = &this->return_ctx; // &this->caller_ctx;
    makecontext(&this->callee_ctx, (void (*)(void))this->function, 1, this);

    return this;
}

void GeneratorFree(Generator* this) {
    free(this);
}

void GeneratorYieldFrom(Generator* this, void (*function)(Generator*)) {
    GeneratorYield(this, NULL);
    function(this);
}

void* GeneratorYield(Generator* this, void* value) {
    this->value = value;
    swapcontext(&this->callee_ctx, &this->caller_ctx);
    void* message = this->message;
    this->value = NULL;
    this->message = NULL;
    return message;
}

void* GeneratorNext(Generator* this, void* message) {
    if (!this->done) {
        this->message = message;
        swapcontext(&this->caller_ctx, &this->callee_ctx);
        void* value = this->value;
        this->value = NULL;
        return value;
    }
    return NULL;
}

