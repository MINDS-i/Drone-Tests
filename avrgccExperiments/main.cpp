#include <avr/io.h>
#include <util/delay.h>
#include <avr/sleep.h>
#include <stdio.h>



#if 1 // just to fix sublime text font coloring glitch
// these registers are reserved on the atmega2560,
// so they will not interfere with anything
#define DBGS _SFR_IO8(0x38)
#define DBGD _SFR_IO8(0x39)
#endif

//configure printf to write up to emulator in a constructor before main
__attribute__((constructor))
void init_io() {
    fdevopen( [](char c, FILE * f) -> int { DBGD = c; return 0; }, NULL );
}
//The simulator is configured to stop when sleep mode is activated
__attribute__((destructor))
void sleep_after_exit() {
    sleep_mode();
}

const uint8_t TEST_START  = 3;
const uint8_t CLOCK_START = 2;
const uint8_t CLOCK_END   = 1;
const uint8_t TEST_END    = 0;

#define beginTest(testName, ...) \
    do { \
        DBGS = TEST_START; \
        printf(testName, ##__VA_ARGS__); \
        DBGS = CLOCK_START; \
    } while(0)

#define endTest(results, ...) \
    do { \
        DBGS = CLOCK_END; \
        printf(results, ##__VA_ARGS__); \
        DBGS = TEST_END; \
    } while(0)

int main(void){

    beginTest("overhead");
    endTest("Passed");

    beginTest("Square root of 12.0");
    volatile float x = 2.0;
    volatile float y = sqrt(x);
    endTest("Passed, was %f\n", y);

    return 0;
}
