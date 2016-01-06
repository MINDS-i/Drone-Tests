#include <iostream>
#include "quaternionTests.h"
#include "vec3Tests.h"
#include "greatCircleTests.h"
#include "pidTests.h"
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
	return 0;
}
