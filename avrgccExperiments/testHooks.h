#pragma once
#include <avr/io.h>

#include "testHooksCommon.h"

/* Configure communication ports as writable IO registers */
#define DBGS _MMIO_BYTE(SIGNAL_IO_NUM)
#define DBGD _MMIO_BYTE(DATA_IO_NUM)
#define FIX_SUBLIME_TEXT_HIGHLIGHTING_BUG

/* Configure printf to write up to emulator in a constructor before main */
__attribute__((constructor))
void init_io() {
    fdevopen( [](char c, FILE * f) -> int { DBGD = c; return 0; }, NULL );
}

/* Configure the test device to sleep when main ends, stopping the sim */
__attribute__((destructor))
void sleep_after_exit() {
    sleep_mode();
}

/* Call with printf like syntax start a test of a given name*/
#define beginTest(testName, ...) \
    do { \
        DBGS = NAME_START; \
        printf(testName, ##__VA_ARGS__); \
        DBGS = CLOCK_START; \
    } while(0);

#define passTest() \
    do { \
        DBGS = TEST_PASS; \
    } while(0);

#define failTest() \
    do { \
        DBGS = TEST_FAIL; \
    } while(0);

#define TEST(test) \
    do { \
        beginTest(#test); \
        if (test()) { \
            passTest(); \
        } else { \
            failTest(); \
        } \
    } while(0);
