#include <stdio.h>
#include "./include/Generator.h"
#define GeneratorActive(g) (g->done == false)
// http://albertnetymk.github.io/2014/12/05/context/

void once(Generator* gen) {
    printf("once start\n");
    GeneratorYield(gen, NULL);
    printf("once end\n");
}

void count(Generator* gen) {
    printf("count start\n");

    Generator* once_gen = GeneratorMake(once);
    while (!once_gen->done) {
        GeneratorNext(once_gen);
    }
    GeneratorFree(once_gen);

    for (int i = 0; i < 3; i++) {
        printf("i: %d\n", i);
        GeneratorYield(gen, (void*)&i);
    }
    printf("count done\n");
}

int main() {
    printf("main start\n");

    Generator* gcount = GeneratorMake(count);
    int i;
    while (GeneratorActive(gcount)) {
        printf("calling generator next\n");
        i = *((int*)GeneratorNext(gcount));
    }
    GeneratorFree(gcount);

    printf("main done\n");
    return 0;
}
