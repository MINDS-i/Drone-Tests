#include <avr/io.h>
#include <util/delay.h>
#include <avr/sleep.h>
#include <stdio.h>

#include "testHooks.h"

bool executorOverhead(void){
    return true;
}

bool sqrtOfTwelve(void){
    volatile float x = 12.0;
    volatile float y = sqrt(x);
    return y == 1.41421356237f;
}

bool debugMessage(void){
    printf("Debug Message #%i here", 1);
    return false;
}

bool longMessage(void){
    for(int i=0; i<10; i++){
        printf("Debug Message #%i here\n", i);
    }
    return false;
}

int main(void){
    beginTest("basic overhead");
    passTest();

    TEST(executorOverhead);
    TEST(sqrtOfTwelve);
    TEST(debugMessage);
    TEST(longMessage);

    beginTest("manual run");
    sqrtOfTwelve();
    passTest();

    return 0;
}
