#include <avr/io.h>
#include <util/delay.h>
#include <avr/sleep.h>
#include <stdio.h>

#include "testHooks.h"

#include "math/Vec3.h"
#include "input/AxisTranslator.h"
//TESTING "math/Vec3.cpp"

Vec3 axis[]{
    Vec3(0,0,0),
    Vec3(1,0,0),
    Vec3(0,1,0),
    Vec3(0,0,1),
    Vec3(1,1,1),
    Vec3(0,-1,-1),
    Vec3(-1,0,-1),
    Vec3(-1,-1,0),
};
const int numAxis = sizeof(axis)/sizeof(axis[0]);

bool vec3cmp(Vec3& a, Vec3& b){
    return fuzzyCompare(a[0],b[0]) &&
           fuzzyCompare(a[1],b[1]) &&
           fuzzyCompare(a[2],b[2])  ;
}

bool identity(void){
    Translator t = Translators::identity;
    for(int i=0; i<numAxis; i++){
        float input[3] = {axis[i][0], axis[i][1], axis[i][2]};
        Vec3 result = t(input);
        ASSERT(vec3cmp(result, axis[i]));
    }
    return true;
}

bool APMboardConversion(void){
    Translator t = Translators::APM;
    for(int i=0; i<numAxis; i++){
        float input[3] = {axis[i][1], axis[i][0], -axis[i][2]};
        Vec3 result = t(input);
        ASSERT(vec3cmp(result, axis[i]));
    }
    return true;
}

int main(void){
    TEST(identity);
    TEST(APMboardConversion);
    return 0;
}
