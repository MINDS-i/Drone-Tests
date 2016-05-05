#include <avr/io.h>
#include <util/delay.h>
#include <avr/sleep.h>
#include <stdio.h>

#include "Servo.h"
#include "Arduino.h"
#include "testHooks.h"

int main(void){
    beginTest("Servo printing");

    Servo s;
    s.attach(6);
    s.write(90);
    for(volatile uint16_t i=0; i<0xfffe; i++) ;

    passTest();
    return 0;
}
