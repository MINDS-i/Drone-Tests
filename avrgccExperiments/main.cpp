#include <avr/io.h>
#include <util/delay.h>
#include <avr/sleep.h>

#if 1 // just to fix sublime text font coloring glitch
// these registers are reserved on the atmega2560,
// so they will not interfere with anything
#define DBGS _SFR_IO8(0x38)
#define DBGD _SFR_IO8(0x39)
#endif

void test(){
    DBGS = 0x00;
    DBGS = 0x00;
    DBGS = 0x00;
    DBGS = 0x00;
    _delay_ms(1000);
    DBGS = 0x01;
}

int main(void){
    DDRB = 0xFF; // set part B as output
    test();

    sleep_mode();
    return 0;
}
