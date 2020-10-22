

function* Gen1337() {
    return 1337;
}

function* EchoOnce() {
    let value = yield 1;
    return value;
}

function* RecvRange() {
    for (let i = 1; i < 6; i++) {
        yield i;
    }
    return 9;
}

function* SendRecvRange() {
    let value = 0;
    let tmp = 0;
    for (let i = 0; i < 5; i++) {
        tmp = i + value;
        value = yield tmp;
    }
}

function* YieldFrom(gen) {
    yield* gen;
}


function test0() {
    console.log("testing single value receive");
    let gen = Gen1337();
    return gen.next().value === 1337;
}

function test1() {
    console.log("testing single value echo");
    let tmp = null;
    let success = true;
    let echo = EchoOnce();

    tmp = echo.next(1234).value;
    success = tmp === 1;

    tmp = echo.next(5678).value;
    success = success && tmp === 5678;

    return success;
}


function test2() {
    console.log("basic value iteration");
    let numbers = RecvRange();
    let value;
    for (;;) {
        const it = numbers.next();
        if (it.done) {
            break;
        }
        value = it.value;
    }
    return value === 5;
}

function test3() {
    console.log("basic send/receive iteration");
    const gen = SendRecvRange();
    let val = 0;
    for (let i = 100;;i-=2) {
        const it = gen.next(i);
        if (it.done) {
            break;
        }
        val = it.value;
    }
    return val === 96;
}

function test4() {
    console.log("basic yield from");
    let val;
    for (const value of YieldFrom(RecvRange())) {
        val = value;
    }
    return val === 5;
}

function test5() {
    console.log("send/receive yield from");
    const gen = YieldFrom(SendRecvRange());
    let value = 0;
    for (let i = 1; ; i++) {
        const it = gen.next(i);
        if (it.done) {
            break;
        }
        value = it.value;
    }
    return value === 9;
}

function main() {

    if (!test0()) {
        throw "FAILED TEST 0";
    } else {
        console.info("PASS\n");
    }

    if (!test1()) {
        throw "FAILED TEST 1";
    } else {
        console.info("PASS\n");
    }

    if (!test2()) {
        throw "FAILED TEST 2";
    } else {
        console.info("PASS\n");
    }

    if (!test3()) {
        throw "FAILED TEST 3";
    } else {
        console.info("PASS\n");
    }

    if (!test4()) {
        throw "FAILED TEST 4";
    } else {
        console.info("PASS\n");
    }

    if (!test5()) {
        throw "FAILED TEST 5";
    } else {
        console.info("PASS\n");
    }

    console.log("SUCCESS - ALL TESTS PASSED");

}

main();
