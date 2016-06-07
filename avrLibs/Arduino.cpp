#include "Arduino.h"
#include "testHooks.h"
#include <avr/interrupt.h>

uint32_t _falseTime;
uint32_t micros(){
	return _falseTime;
}
uint32_t millis(){
	return (_falseTime/1000UL);
}
void advanceMicros(uint32_t us){
	_falseTime += us;
}
void advanceMillis(uint32_t ms){
	_falseTime += ms*1000;
}
void setMicros(uint32_t us){
	_falseTime = us;
}
void setMillis(uint32_t ms){
	_falseTime = ms*1000;
}
bool fuzzyCompare(float a, float b, float diff){
    float absA  = fabs(a);
    float absB  = fabs(b);
    float error = fabs(a - b);

    if(b == 0 || a == 0) return error < diff;
    return (error/(absA+absB)) < diff;
}

/* Configure printf to write up to emulator in a constructor before main */
__attribute__((constructor))
void init_io() {
    fdevopen( [](char c, FILE * f) -> int { DBGD = c; return 0; }, NULL );
    sei();
}

/* Configure the test device to sleep when main ends, stopping the sim */
__attribute__((destructor))
void sleep_after_exit() {
    cli();
    sleep_mode();
}

extern "C" {
    void __cxa_pure_virtual(void) {
      printf("ERROR __cxa_pure_virtual");
      abort();
    }
    void __cxa_deleted_virtual(void) {
      printf("ERROR __cxa_deleted_virtual");
      abort();
    }
}

