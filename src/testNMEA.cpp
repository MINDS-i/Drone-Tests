#include <avr/io.h>
#include <util/delay.h>
#include <avr/sleep.h>
#include <stdio.h>

#include "testHooks.h"
#include "TestStreams.h"

#include "comms/NMEA.h"
//TESTING "comms/NMEA.cpp"
//TESTING "math/GreatCircle.cpp"
//TESTING "math/Waypoint.cpp"
//TESTING "math/SpatialMath.cpp"

bool TestStream(void){
    StringStream ss("I'ma String Stream");
    ss.write((const uint8_t *)"Hello there terminal!", 21);
    while(ss.available()){
        ss.write(ss.read());
    }
    return ss.read() == -1;

}

int main(void){
    TEST(TestStream);
    return 0;
}
