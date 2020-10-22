#include <stdio.h>
#include <stdbool.h>
#include "./include/Generator.h"
#define GeneratorActive(g) (g->done == false)
// http://albertnetymk.github.io/2014/12/05/context/


void count(Generator* gen) {
    printf("count start\n");
    for (int i = 0; i < 3; i++) {
        char msg = *((char*)GeneratorYield(gen, (void*)&i));
        //printf("received %c\n", msg);
    }
    printf("count done\n");
    GeneratorReturn(gen, NULL);
}


void passthru(Generator* gen) {
    //printf("passthru start\n");

    //static int something = 123;
    //static int something_else = 456;
    //char* g = (char*)GeneratorYield(gen, (void*)&something);
    //printf("received %c\n", *g);

    //// yield from
    //Generator* gcount = GeneratorMake(count);
    //GeneratorYieldFrom(gen, gcount);
    //GeneratorFree(gcount);

    //printf("passthru end\n");
    //GeneratorReturn(gen, &something_else);
}


void Gen1337(Generator* gen) {
    static int val = 1337;
    GeneratorReturn(gen, (void*)&val);
}

void EchoOnce(Generator* gen) {
    int a = 1;
    int b = 0;
    b = *((int*)GeneratorYield(gen, (void*)&a));
    GeneratorReturn(gen, &b);
}

void RecvRange(Generator* gen) {
    static int i = 0;
    for (i = 1; i < 6; i++) {
        GeneratorYield(gen, (void*)&i);
    }
}

void SendRecvRange(Generator* gen) {
    int i = 0;
    int tmp = 0;
    int value = 0;
    for (i = 0; i < 5; i++) {
        tmp = i + value;
        value = *(int*)GeneratorYield(gen, (void*)&tmp);
    }
}

void YieldFrom(Generator* gen) {
    void (*other)(Generator*) = (void(*)(Generator*))GeneratorYield(gen, NULL);
    GeneratorYieldFrom(gen, other);
}

bool test0() {
    printf("testing single value receive\n");
    Generator* gen = GeneratorMake(Gen1337);
    int* itval = (int*)GeneratorNext(gen, NULL);
    int val = *itval;
    GeneratorFree(gen);
    return val == 1337;
}

bool test1() {
    int val_1 = 1234;
    int val_2 = 5678;
    int *tmp = NULL;
    printf("testing single value echo\n");

    bool success = true;
    Generator* echo = GeneratorMake(EchoOnce);

    tmp = (int*)GeneratorNext(echo, (void*)&val_1);
    success = *tmp == 1;

    tmp = (int*)GeneratorNext(echo, (void*)&val_2);
    success = success && *tmp == 5678;

    GeneratorFree(echo);
    return success;
}

bool test2() {
    printf("basic value iteration\n");

    Generator* numbers = GeneratorMake(RecvRange);
    int value;
    for (;;) {
        const int* itvalue = (int*)GeneratorNext(numbers, NULL);
        if (numbers->done) {
            break;
        }
        if (itvalue != NULL) {
            value = *itvalue;
        }
    }
    return value == 5;
}

bool test3() {
    printf("basic send/receive iteration\n");
    Generator* gen = GeneratorMake(SendRecvRange);
    int val = 0;
    for (int i = 100; ;i-=2) {
        int* itval = GeneratorNext(gen, &i);
        if (gen->done) {
            break;
        }
        val = *itval;
    }
    return val == 96;
}

bool test4() {
    printf("basic yield from\n");
    int val;
    Generator* gen = GeneratorMake((void (*)(Generator*))YieldFrom);
    GeneratorNext(gen, NULL);
    GeneratorNext(gen, RecvRange); // send RecvRange as "argument"
    for (;;) {
        int* value = (int*)GeneratorNext(gen, NULL);
        if (gen->done) {
            break;
        }
        val = *value;
    }
    GeneratorFree(gen);
    return val == 5;
}

// basic yield from
bool test5() {
    printf("send/receive yield from\n");
    Generator* gen = GeneratorMake((void (*)(Generator*))YieldFrom);
    GeneratorNext(gen, NULL);
    GeneratorNext(gen, SendRecvRange);

    int value = 0;
    for (int i = 1; ; i++) {
        int* itval = (int*)GeneratorNext(gen, (void*)&i);
        if (gen->done) {
            break;
        }
        value = *itval;
    }
    return value == 9;
}

int bombout(char* msg) {
    fprintf(stderr, "%s\n", msg);
    return 1;
}

int main() {

    if (!test0()) {
        return bombout("FAILED TEST 0");
    } else {
        printf("PASS\n\n");
    }

    if (!test1()) {
        return bombout("FAILED TEST 1");
    } else {
        printf("PASS\n\n");
    }

    if (!test2()) {
        return bombout("FAILED TEST 1");
    } else {
        printf("PASS\n\n");
    }

    if (!test3()) {
        return bombout("FAILED TEST 3");
    } else {
        printf("PASS\n\n");
    }

    if (!test4()) {
        return bombout("FAILED TEST 4");
    } else {
        printf("PASS\n\n");
    }

    if (!test5()) {
        return bombout("FAILED TEST 5");
    } else {
        printf("PASS\n\n");
    }

    printf("SUCCESS - ALL TESTS PASSED\n");
    return 0;
}
    //Generator* passthru_g = GeneratorMake(passthru, 0);
    //while (GeneratorActive(passthru_g)) {
    //    int* tmp = (int*)GeneratorNext(passthru_g, (void*)&c);
    //    if (tmp == NULL) {
    //        printf("when I sent %c, count responded NULL\n", c);
    //    } else {
    //        printf("tmp: %p, ", tmp);
    //        i = *tmp;
    //        printf("when I sent %c, count responded %d\n", c, i);
    //    }
    //    c++;
    //}
    //GeneratorFree(passthru_g);

