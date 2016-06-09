#include "testHooks.h"
#include "math/Algebra.h"

/**
     * Built from a cubic function (a*x^3 + b*x^2 + c*x + d) defined at
     * f(-1) = 0
     * f( 1) = 1
     * f( 0) = hoverPoint
     * with the last degree of freedom setting the x^3 vs x balance
     *     which corresponds to how "linear" the curve is
     */

bool testNoThrottle(){
    ThrottleCurve curves[] = {
        ThrottleCurve(0.5, 0.5),
        ThrottleCurve(0.2, 0.5),
        ThrottleCurve(0.5, 0.2),
        ThrottleCurve(0.8, 0.5),
        ThrottleCurve(0.5, 0.8),
    };
    FPASSERT(curves[0].get(0.0), 0.0);
    FPASSERT(curves[1].get(0.0), 0.0);
    FPASSERT(curves[2].get(0.0), 0.0);
    FPASSERT(curves[3].get(0.0), 0.0);
    FPASSERT(curves[4].get(0.0), 0.0);
}

bool testFullThrottle(){
    ThrottleCurve curves[] = {
        ThrottleCurve(0.5, 0.5),
        ThrottleCurve(0.2, 0.5),
        ThrottleCurve(0.5, 0.2),
        ThrottleCurve(0.8, 0.5),
        ThrottleCurve(0.5, 0.8),
    };
    FPASSERT(curves[0].get(1.0), 1.0);
    FPASSERT(curves[1].get(1.0), 1.0);
    FPASSERT(curves[2].get(1.0), 1.0);
    FPASSERT(curves[3].get(1.0), 1.0);
    FPASSERT(curves[4].get(1.0), 1.0);
}

bool testHoverPoint(){
    ThrottleCurve curves[] = {
        ThrottleCurve(0.5, 0.5),
        ThrottleCurve(0.2, 0.5),
        ThrottleCurve(0.5, 0.2),
        ThrottleCurve(0.8, 0.5),
        ThrottleCurve(0.5, 0.8),
    };
    FPASSERT(curves[0].get(0.5), 0.5);
    FPASSERT(curves[1].get(0.5), 0.5);
    FPASSERT(curves[2].get(0.5), 0.2);
    FPASSERT(curves[3].get(0.5), 0.5);
    FPASSERT(curves[4].get(0.5), 0.8);
}

void benchmarkCalculate(){
    volatile float x = 0.1777;
    x = ThrottleCurve(0.25, 0.4).get(x);
}

int main(void){
    TEST(testNoThrottle);
    TEST(testFullThrottle);
    TEST(testHoverPoint);
    BENCHMARK(benchmarkCalculate);
    return 0;
}
