#pragma once
#include <avr/io.h>
#include <avr/sleep.h>
#include <stdio.h>
#include <math.h>
#include <float.h>

#include "testHooksCommon.h"
#include "Arduino.h"
/* Configure communication ports as writable IO registers */
#define DBGS _MMIO_BYTE(SIGNAL_IO_NUM)
#define DBGD _MMIO_BYTE(DATA_IO_NUM)
#define FIX_SUBLIME_TEXT_HIGHLIGHTING_BUG

/* Call with printf syntax to start a test of a given name */
#define beginTest(testName, ...) \
    do { \
        DBGS = NAME_START; \
        printf(testName, ##__VA_ARGS__); \
        DBGS = CLOCK_START; \
    } while(0)

#define passTest() \
    do { \
        DBGS = TEST_PASS; \
    } while(0)

#define failTest() \
    do { \
        DBGS = TEST_FAIL; \
    } while(0)

#define TEST(test) \
    do { \
        beginTest(#test); \
        if (test()) { \
            passTest(); \
        } else { \
            failTest(); \
        } \
    } while(0)

#define ASSERT(a) \
    do { \
        if(!a) { \
            printf("%s was false", #a); \
            return false; \
        } \
    } while(0)

#define CASSERT(a,b) \
    do { \
        if(!((a) == (b))) { \
            printf("%s != %s", #a, #b); \
            return false; \
        } \
    } while(0)

#define FPASSERT(a,b) \
    do { \
        const float ans = a; \
        const float chk = b; \
        if(!fuzzyCompare(ans,chk)) { \
            printf("%s=%f != %f", #a, ans, chk); \
            return false; \
        } \
    } while(0)

#define FPASSERTC(a,b,c) \
    do { \
        const float ans = a; \
        const float chk = b; \
        if(!fuzzyCompare(a,b,c)) { \
            printf("%s=%f != %f", #a, ans, chk); \
            return false; \
        } \
    } while(0)
