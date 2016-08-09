#include "testHooks.h"
#include "math/Waypoint.h"
#include "math/SpatialMath.h"
//TESTING "math/Waypoint.cpp"

const float EPS = 0.001; //allowable error epsilon for floating point ops

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
        FPASSERT(response, testAngles[i][1]*rev);
    }
    return true;
}
bool GCtruncateRadian(){
    const float rev = 2.0f*M_PI;
    for(int i=0; i<NUM_TEST_ANGLES; i++){
        float response = truncateRadian(testAngles[i][0]*rev);
        FPASSERT(response, testAngles[i][1]*rev);
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

typedef struct TestPath {
    Waypoint start;
    Waypoint end;
    float    heading;
    float    distance;
} TestPath;

const TestPath testPaths[]{
    //start,       end,  heading, distance,
    {origin,     north,      0.0, 69.0934},
    {origin, northEast,   44.995, 97.7103},
    {origin,      east,     90.0, 69.0934},
    {origin, southEast,  135.004, 97.7103},
    {origin,     south,    180.0, 69.0934},
    {origin, southWest, -135.004, 97.7103},
    {origin,      west,    -90.0, 69.0934},
    {origin, northWest,  -44.995, 97.7103}
};
const int numPaths = sizeof(testPaths)/sizeof(testPaths[0]);

bool GCcalcHeading(){
    for(int i=0; i<numPaths; i++){
        TestPath t = testPaths[i];
        FPASSERTC(t.start.headingTo(t.end), t.heading, EPS);
            //calcHeading(t.start, t.end), t.heading, EPS);
    }
    return true;
}
bool GCcalcDistance(){
    for(int i=0; i<numPaths; i++){
        TestPath t = testPaths[i];
        FPASSERTC(t.start.distanceTo(t.end), t.distance, EPS);
            //calcDistance(t.start, t.end), t.distance, EPS);
    }
    return true;
}

bool GCextrapPosition(){
    for(int i=0; i<numPaths; i++){
        TestPath t = testPaths[i];
        Waypoint result = t.start.extrapolate(t.heading, t.distance);
            //extrapPosition(t.start, t.heading, t.distance);
        FPASSERTC(result.distanceTo(t.end), 0.0, 2*EPS);
            //calcDistance(result, t.end), 0.0, 2*EPS);
    }
    return true;
}

int main(){
    TEST(GCtruncateDegree   );
    TEST(GCtruncateRadian   );
    TEST(GCcalcHeading      );
    TEST(GCcalcDistance     );
    TEST(GCextrapPosition   );
}
