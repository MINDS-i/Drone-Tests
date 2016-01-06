#ifndef GREAT_CIRCLE_TESTS_H
#define GREAT_CIRCLE_TESTS_H
#include "test.h"
#include "math/GreatCircle.h"
#include <iostream>
/*
remaining to test:
float simplifyRadian(float ref, float val);
float simplifyDegree(float ref, float val);
inline float distanceRadian(float a, float b){ return simplifyRadian(0, b-a); }
inline float distanceDegree(float a, float b){ return simplifyDegree(0, b-a); }
Waypoint extrapPosition(Waypoint position, float bearing, float distance);
*/
namespace GreatCircleTests{
    const int NUM_TEST_ANGLES = 12;
    const float testAngles[NUM_TEST_ANGLES][2]{
         {  0.0,  0.0} //arg, response
        ,{ 0.25, 0.25}
        ,{ 0.50, 0.50}
        ,{ 0.75,-0.25}
        ,{ 1.00, 0.00}
        ,{ 1.25, 0.25}
        ,{ 5.25, 0.25}
        ,{-0.25,-0.25}
        ,{-0.50, 0.50}
        ,{-0.75, 0.25}
        ,{-1.00, 0.00}
        ,{-1.25,-0.25}
    };
    bool GCtruncateDegree(){
        const float rev = 360.0;
        for(int i=0; i<NUM_TEST_ANGLES; i++){
            float response = truncateDegree(testAngles[i][0]*rev);
            if( !fuzzyCompare(response, testAngles[i][1]*rev) ) return false;
        }
        return true;
    }
    bool GCtruncateRadian(){
        const float rev = 2.0f*M_PI;
        for(int i=0; i<NUM_TEST_ANGLES; i++){
            float response = truncateRadian(testAngles[i][0]*rev);
            if( !fuzzyCompare(response, testAngles[i][1]*rev) ) return false;
        }
        return true;
    }
    const Waypoint origin     ( 0, 0);
    const Waypoint north      ( 1, 0);
    const Waypoint northEast  ( 1, 1);
    const Waypoint east       ( 0, 1);
    const Waypoint southEast  (-1, 1);
    const Waypoint south      (-1, 0);
    const Waypoint southWest  (-1,-1);
    const Waypoint west       ( 0,-1);
    const Waypoint northWest  ( 1,-1);
    bool GCcalcHeading(){
        FPASSERTC(calcHeading(origin, north       ), 0.  , 0.005);
        FPASSERTC(calcHeading(origin, northEast   ), 45. , 0.005);
        FPASSERTC(calcHeading(origin, east        ), 90. , 0.005);
        FPASSERTC(calcHeading(origin, southEast   ), 135., 0.005);
        FPASSERTC(calcHeading(origin, south       ), 180., 0.005);
        FPASSERTC(calcHeading(origin, southWest   ),-135., 0.005);
        FPASSERTC(calcHeading(origin, west        ),-90. , 0.005);
        FPASSERTC(calcHeading(origin, northWest   ),-45. , 0.005);
        return true;
    }
    bool GCcalcDistance(){
        FPASSERTC(calcDistance(origin, north    ), 69.093, 0.005);
        FPASSERTC(calcDistance(origin, northEast), 97.710, 0.005);
        FPASSERTC(calcDistance(origin, south    ), 69.093, 0.005);
        FPASSERTC(calcDistance(origin, southWest), 97.710, 0.005);
        return true;
    }
    bool GCextrapPosition(){

        for(int i=0; i<12; i++){
            const float MAX = 90;
            Waypoint start(randF(-MAX, MAX), randF(-MAX, MAX));
            Waypoint finish(randF(-MAX, MAX), randF(-MAX, MAX));
            float h = calcHeading(start, finish);
            float d = calcDistance(start, finish);
            Waypoint result = extrapPosition(start, h, d);
            float error = calcDistance(result, finish);

            if(!fuzzyCompare(error, 0.0, 0.005)){
                cout << start.lat << "\t" << start.lng << endl;
                cout << finish.lat << "\t" << finish.lng << endl;
                cout << "distance: " << d << " heading: " << h << endl;
                cout << result.lat << "\t" << result.lng << endl;
                cout << "error of " << error << endl;
                return false;
            }
        }
        return true;
    }

    void runTests(){
        TEST(GCtruncateDegree   );
        TEST(GCtruncateRadian   );
        TEST(GCcalcHeading      );
        TEST(GCcalcDistance     );
        TEST(GCextrapPosition   );
        //TEST(                   );
    }
}
#endif
