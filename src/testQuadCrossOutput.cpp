#include <avr/io.h>
#include <util/delay.h>
#include <avr/sleep.h>
#include <stdio.h>

#include "testHooks.h"

#include "output/QuadCrossOutput.h"

/*
cross quad copter input model:
[1 1 -1 -1
 1 -1 -1 1
 1 -1 1 -1
 1 1 1 1]
*/
const float testData[][2][4] {
    {{0.0,0.0,0.0,0.0},{ 0.00, 0.00, 0.00, 0.00}},
    {{1.0,0.0,0.0,0.0},{ 0.25, 0.25,-0.25,-0.25}},
    {{0.0,1.0,0.0,0.0},{ 0.25,-0.25,-0.25, 0.25}},
    {{0.0,0.0,1.0,0.0},{ 0.25,-0.25, 0.25,-0.25}},
    {{0.0,0.0,0.0,1.0},{ 0.25, 0.25, 0.25, 0.25}},
    {{1.0,1.0,1.0,1.0},{ 1.00, 0.00, 0.00, 0.00}},
};
int numTestData = sizeof(testData)/sizeof(testData[0]);

bool crossOutputExpected(void){
    for(int i=0; i<numTestData; i++){
        float result[4];
        solveOutputs(testData[i][0], result);
        for(int j=0; j<4; j++){
            FPASSERT(testData[i][1][j], result[j]);
        }
    }
}

int main(void){
    float result[4];

    TEST(crossOutputExpected);

    float zeros[4] = {0.0, 0.0, 0.0, 0.0};
    beginTest("Benchmark 0's");
    solveOutputs(zeros ,result);
    benchFinish();

    float ones[4] = {1.0, 1.0, 1.0, 1.0};
    beginTest("Benchmark 1's");
    solveOutputs(ones ,result);
    benchFinish();

    return 0;
}
