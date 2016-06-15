#include "testHooks.h"

#include "filter/OrientationEngine.h"
#include "util/PIDcontroller.h"
#include "util/PIDparameters.h"
#include "filter/RCFilter.h"
#include "output/Horizon.h"
//TESTING "math/Vec3.cpp"
//TESTING "math/Quaternion.cpp"
//TESTING "math/GreatCircle.cpp"
//TESTING "math/SpatialMath.cpp"

PIDparameters pitchI(0.1, 0.0, 0.1);
PIDparameters rollI(0.1, 0.0, 0.1);
PIDparameters yawI(0.1, 0.0, 0.1);
PIDparameters pitchO(0.1, 0.0, 0.1);
PIDparameters rollO(0.1, 0.0, 0.1);
PIDparameters yawO(0.1, 0.0, 0.1);
Horizon horizon(&pitchI, &rollI, &yawI, &pitchO, &rollO, &yawO);
RCFilter engine(1.0, 1.0);

const float DELTA_T = 5.0; // elapsed time is milliseconds to report

bool noOutputInStandby(){
	float torques[4] = {1.0, 1.0, 1.0, 1.0};

	horizon.standby();
	horizon.update(engine, DELTA_T, torques);
	for(int i=0; i<4; i++){
		FPASSERT(torques[i], 0.0);
	}
	return true;
}

bool regularOutput(){
	float torques[4];
	horizon.activate();
	horizon.set(0.5, 0, 0, 0.5);
	horizon.update(engine, DELTA_T, torques);
	return true;
}

int main(){
	TEST(noOutputInStandby);

	float torques[4];
	horizon.activate();
	horizon.set(0.5, 0, 0, 0.5);
	beginTest("Horizon calculation");
	horizon.update(engine, DELTA_T, torques);
	benchFinish();
}
