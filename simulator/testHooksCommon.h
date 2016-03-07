#pragma once

#include <stdio.h>

enum Signal : uint8_t {
    NAME_START,
    CLOCK_START,
    TEST_PASS,
    TEST_FAIL,
};

/* These two IO addresses are reserved and unused on the atmega2560 */
#define SIGNAL_IO_NUM 0x58
#define DATA_IO_NUM   0x59

/*
 * Test data is transmitted by the AVR following this sequence:
 *  > set the SIGNAL register to NAME_START
 *  > write the test name to the DATA buffer
 *  > set the SIGNAL register to CLOCK_START
 *  > set the SIGNAL register to either TEST_PASS or TEST_FAIL
 * The clock stops when either TEST_PASS or TEST_FAIL is received.
 * Any messages written to DATA buffer when SIGNAL != NAME_START should be
 *   printed to the output console.
 */
