#include <iostream>
#include "quaternionTests.h"
#include "vec3Tests.h"
#include "greatCircleTests.h"
#include "pidTests.h"
#include "bufferTest.h"
#include "decoderTest.h"
using namespace std;
int main(int argc, char const *argv[]) {
    cout << "Vec3 Unit tests: " << endl;
    vec3Tests::runTests();
    cout << "Quaternion Unit tests: " << endl;
	quatTests::runTests();
    cout << "Great Circle Unit Tests: " << endl;
    GreatCircleTests::runTests();
    cout << "PID Tests: " << endl;
    PIDTests::runTests();
    cout << "CircBuf Tests: " << endl;
    CircBufTests::runTests();
    cout << "Decoder Tests: " << endl;
    DecoderTests::runTests();
	return 0;
}
