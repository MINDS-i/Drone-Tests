#include "testHooks.h"
#include "Arduino.h"

#include "util/Interval.h"

bool everyTrueAtInterval(){
    setMillis(0);
    auto interval = Interval::every(50);
    for(int i=0; i<10; i++){
        advanceMillis(51);
        ASSERT(interval() == true);
    }
    return true;
}

bool everyFalseAfterFirstTrigger(){
    setMillis(0);
    auto interval = Interval::every(50);
    advanceMillis(1);
    for(int i=0; i<10; i++){
        advanceMillis(50);
        interval();
        ASSERT(interval() == false);
    }
    return true;
}

bool elapsedFalseBeforeInterval(){
    setMillis(0);
    auto timer = Interval::elapsed(50);
    ASSERT(timer() == false);
    advanceMillis(49);
    ASSERT(timer() == false);
    return true;
}

bool elapsedTrueAfterInterval(){
    setMillis(0);
    auto timer = Interval::elapsed(50);
    advanceMillis(51);
    ASSERT(timer() == true);
    advanceMillis(5000);
    ASSERT(timer() == true);
    return true;
}

int main(){
    TEST(everyTrueAtInterval);
    TEST(everyFalseAfterFirstTrigger);
    TEST(elapsedFalseBeforeInterval);
    TEST(elapsedTrueAfterInterval);
}
