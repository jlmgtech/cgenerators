

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
    for (let i = 0; i < 5; i++) {
        value = yield (i + value);
    }
}

function* YieldFrom(gen) {
    console.log("YieldFrom start");
    yield* gen;
    console.log("YieldFrom end");
}


function test0() {
    console.log("testing single value receive");
    let gen = Gen1337();
    return gen.next().value === 1337;
}

function test1() {
    console.log("testing single value echo");
    let success = true;
    let echo = EchoOnce();
    // the first SEND will be ignored
    success = success && echo.next(1234).value === 1;
    // the second one is yielded as sent
    success = success && echo.next(5678).value === 5678;
    return success;
}


function test2() {
    console.log("basic value iteration");
    let numbers = RecvRange();
    console.log("should count 1 to 5");
    let value;
    for (const value of numbers) {
        console.log(value);
    }
    //for (;;) {
    //    const it = numbers.next();
    //    if (it.done) {
    //        break;
    //    }
    //    value = it.value;
    //    //process.stdout.write(value + " ");
    //    console.log(value);
    //}
    process.stdout.write("\n");
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
