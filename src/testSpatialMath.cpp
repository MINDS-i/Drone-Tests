#include <avr/io.h>
#include <util/delay.h>
#include <avr/sleep.h>
#include <stdio.h>

#include "testHooks.h"

#include "math/SpatialMath.h"
//TESTING "math/SpatialMath.cpp"

const float EPSILON = 0.0000005;

typedef struct TestCase { float rad; float deg; } TestCase;
const TestCase cases[] {
    { -2.0*M_PI/4.0, -90.0},
    { -1.0*M_PI/4.0, -45.0},
    {  0.0*M_PI/4.0,   0.0},
    {  1.0*M_PI/4.0,  45.0},
    {  2.0*M_PI/4.0,  90.0},
    {  3.0*M_PI/4.0, 135.0},
    {  4.0*M_PI/4.0, 180.0},
    {  5.0*M_PI/4.0, 225.0},
    {  6.0*M_PI/4.0, 270.0},
    {  7.0*M_PI/4.0, 315.0},
    {  8.0*M_PI/4.0, 360.0},
    {  9.0*M_PI/4.0, 405.0}
};
const int numTCs = sizeof(cases)/sizeof(cases[0]);

bool toRadTest(){
    for(int i=0; i<numTCs; i++){
        FPASSERTC(toRad(cases[i].deg), cases[i].rad, EPSILON);
    }
}

bool toDegTest(){
    for(int i=0; i<numTCs; i++){
        FPASSERTC(toDeg(cases[i].rad), cases[i].deg, EPSILON);
    }
}

int main(void){
    TEST(toRadTest);
    TEST(toDegTest);
    return 0;
}
