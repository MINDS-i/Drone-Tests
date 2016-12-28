#include <avr/io.h>
#include <util/delay.h>
#include <avr/sleep.h>
#include <stdio.h>

#include "testHooks.h"

#include "output/QuadCrossOutput.h"

/*
 inputs:
 0: pitch ccw+
 1: roll ccw+
 2: z ccw+
 3: summed output force

 outputs:
  ^ Forward ^
  ---     ---
 | 2 |   | 0 |
  ---\ ^ / ---
      XXX
      XXX
  ---/   \---
 | 1 |   | 3 |
  ---     ---
  0 - counter clockwise
  1 - counter clockwise
  2 - clockwise
  3 - clockwise
*/
const float testData[][2][4] {
    {{0.0,0.0,0.0,0.0},{ 0.00, 0.00, 0.00, 0.00}},
    {{1.0,0.0,0.0,0.0},{ 0.25,-0.25, 0.25,-0.25}},
    {{1.0,0.0,0.0,1.0},{ 0.50, 0.00, 0.50, 0.00}},
    {{0.0,1.0,0.0,0.0},{-0.25, 0.25, 0.25,-0.25}},
    {{0.0,1.0,0.0,1.0},{ 0.00, 0.50, 0.50, 0.00}},
    {{0.0,0.0,1.0,0.0},{ 0.25, 0.25,-0.25,-0.25}},
    {{0.0,0.0,1.0,1.0},{ 0.50, 0.50, 0.00, 0.00}},
    {{0.0,0.0,0.0,1.0},{ 0.25, 0.25, 0.25, 0.25}},
};
int numTestData = sizeof(testData)/sizeof(testData[0]);

bool crossOutputExpected(void){
    for(int i=0; i<numTestData; i++){
        float result[4];
        solveOutputs(testData[i][0], result);
        /*
        printf("input %f %f %f %f\n",testData[i][0][0],testData[i][0][1],testData[i][0][2],testData[i][0][3]);
        printf("expct %f %f %f %f\n",testData[i][1][0],testData[i][1][1],testData[i][1][2],testData[i][1][3]);
        printf("reslt %f %f %f %f\n",result[0],result[1],result[2],result[3]);
        printf("\n");
        */
        for(int j=0; j<4; j++){
            FPASSERT(testData[i][1][j], result[j]);
        }
    }
}

int main(void){
    float result[4];

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
