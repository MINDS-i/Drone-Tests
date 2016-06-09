#include "testHooks.h"

#include "filter/OrientationEngine.h"
#include "filter/RCFilter.h"
#include "input/InertialManager.h"
#include "math/Quaternion.h"
#include "math/Vec3.h"
#include "input/AxisTranslator.h"
//TESTING "math/Vec3.cpp"
//TESTING "math/Quaternion.cpp"

bool filterInit(void){
	RCFilter filter(1.0, 1.0);
	return true;
}

bool inertialManagerInit(void){
	InertialManager((InertialVec**)NULL,(Translator*)NULL,0);
	return true;
}

void initAndUpdate(void){
	RCFilter filter(0.177, 0.177);
	InertialManager m((InertialVec**)NULL,(Translator*)NULL,0);
	filter.update(m);
}

int main(){
	TEST(filterInit);
	TEST(inertialManagerInit);
	BENCHMARK(initAndUpdate);
}
