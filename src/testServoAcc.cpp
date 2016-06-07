#include <avr/io.h>
#include <string.h>
#include <util/delay.h>
#include <avr/sleep.h>
#include <avr/interrupt.h>
#include <stdio.h>

#include "Arduino.h"
#include "testHooks.h"
#include "APM/ServoGenerator.h"
//TESTING "APM/ServoGenerator.cpp"

uint8_t pinMask = 0x1;
volatile uint8_t* pinReg = &PORTD;

bool pinIsLow(){
    return ((*pinReg & pinMask) == 0);
}

bool pinIsHigh(){
    return ((*pinReg & pinMask) != 0);
}

bool timeSetPinLong(void) {
    *pinReg |= pinMask;
    return true;
}

bool timeSetPinFast(void) {
    PORTD |= 0x1;
    return true;
}

bool timeClearPinLong(void) {
    *pinReg &= ~pinMask;
    return true;
}

bool timeClearPinFast(void) {
    *pinReg &= ~pinMask;
    return true;
}

class Output{
public:
    Output(): highTime(0xffff), pinMask(0), pinReg(0) {}
    uint16_t highTime;
    uint8_t pinMask;
    volatile uint8_t* pinReg;
    bool enabled() const volatile { return pinMask != 0; }
    void disable() volatile { pinMask = 0; pinReg = 0; highTime = 0xffff; }
    void setPin(int p) volatile {
        pinMask = 0x1;
        pinReg = &PORTD;
    }
    void setHigh() const volatile { *pinReg |= pinMask; }
    void setLow() const volatile { *pinReg &= ~pinMask; }
};

constexpr uint8_t MAX_OUTPUTS = 8;
Output output[MAX_OUTPUTS];
uint8_t activeOutputs = 0;

class Action{ //lawsuit
public:
    uint16_t time;
    uint8_t channel;
};
Action actions[MAX_OUTPUTS+1];
Action * next = actions;
void setupActions(){
    actions[MAX_OUTPUTS] = {0xffff, 0};
    for(uint8_t i=0; i<MAX_OUTPUTS; i++){
        actions[i] = {0xffff, i};
    }
}

volatile uint16_t& OCRA = OCR1A;
volatile uint16_t& TCNT = OCR1B; //TCNT isn't writable in the sim for some reason
volatile uint8_t& TIFR = TIFR1;

ISR(TIMER1_COMPA){
    do {
        output[next->channel].setLow();
        next++;
        uint16_t t = next->time;
        OCRA = t;

        if(t > TCNT) break;
        else TIFR &= ~_BV(OCF1A);
    } while(true);
}

bool sort(void){
    //update each channels highTime; 0xffff if the channel is off
    for(uint8_t i=0; i<MAX_OUTPUTS; i++){
        actions[i].time = output[actions[i].channel].highTime;
    }
    //run insertion sort on actions[]; optimised with a cycle count benchmark
    for(uint8_t i=1; i<MAX_OUTPUTS; i++){
        //load the time and channel of action[i] only when needed
        uint16_t time = actions[i].time;
        //quickly check for sorted input
        if(time >= actions[i-1].time) continue;
        //swap up the data to make room for actions[i]
        uint8_t channel = actions[i].channel;
        uint8_t j = i;
        while(j>0 && time < actions[j-1].time){
            actions[j] = actions[j-1];
            j--;
        }
        //put the data from actions[i] back into actions[j]
        actions[j].time = time;
        actions[j].channel = channel;
    }
    return true;
}

bool actionsSorted(void){
    for(int i=0; i<MAX_OUTPUTS; i++){
        if(actions[i].time != i) return false;
    }
    return true;
}

int main(void){
    setupActions();
    for(int i=0; i<MAX_OUTPUTS; i++){
        actions[i].time = 0x0A00;
    }
    next=actions;
    TCNT = 0;
    BENCHMARK_NAMED(TIMER1_COMPA, "TIMER1_COMPA (one action)");

    setupActions();
    for(int i=0; i<MAX_OUTPUTS; i++){
        actions[i].time = 0x0A00;
    }
    next=actions;
    TCNT = 0x0F00;
    BENCHMARK_NAMED(TIMER1_COMPA, "TIMER1_COMPA (all actions)");

    setupActions();
    for(int i=0; i<MAX_OUTPUTS; i++){
        output[i].highTime = i;
    }
    BENCHMARK_NAMED(sort, "sort (sorted input)");
    TEST(actionsSorted);

    setupActions();
    for(int i=0; i<MAX_OUTPUTS; i++){
        output[i].highTime = MAX_OUTPUTS-i-1;
    }
    BENCHMARK_NAMED(sort, "sort (inverse sorted input)");
    TEST(actionsSorted);

    timeClearPinLong();
    TEST(timeSetPinFast);
    TEST(pinIsHigh);

    timeClearPinLong();
    TEST(timeSetPinLong);
    TEST(pinIsHigh);

    timeSetPinLong();
    TEST(timeClearPinFast);
    TEST(pinIsLow);

    timeSetPinLong();
    TEST(timeClearPinLong);
    TEST(pinIsLow);

    return 0;
}
