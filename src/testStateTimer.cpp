#include "testHooks.h"
#include "Arduino.h"

#include "util/StateTimer.h"

bool trueFor(){
    setMillis(0);
    StateTimer timer([](){ return true; });
    ASSERT(timer.trueFor(200) == false);
    advanceMillis(201);
    ASSERT(timer.trueFor(200) == true);
    return true;
}

bool falseFor(){
    setMillis(0);
    StateTimer timer([](){ return false; });
    ASSERT(timer.falseFor(200) == false);
    advanceMillis(201);
    ASSERT(timer.falseFor(200) == true);
    return true;
}

bool updateResetsTime(){
    static bool flag = true;
    setMillis(0);
    StateTimer timer([](){ return flag; });
    ASSERT(timer.trueFor(200) == false);
    advanceMillis(201);
    flag = false;
    timer.update();
    flag = true;
    ASSERT(timer.trueFor(200) == false);
    return true;
}

int main(){
    TEST(trueFor);
    TEST(falseFor);
    TEST(updateResetsTime);
}
