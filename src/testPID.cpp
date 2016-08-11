#include "testHooks.h"
#include "Arduino.h"
#include "util/PIDcontroller.h"
#include "util/PIDparameters.h"
bool PIDporp(){
    PIDparameters params(1,0,0);
    PIDcontroller pid(&params);
    setMicros(0);
    pid.set(0.0);
    advanceMicros(1000);
    FPASSERT(pid.update(0), 0.0);
    advanceMicros(1000);
    FPASSERT(pid.update(1),-1.0);
    advanceMicros(1000);
    FPASSERT(pid.update(2),-2.0);
    advanceMicros(1000);
    FPASSERT(pid.update(-100), 100.0);
    return true;
}

bool PIDintegral(){
    PIDparameters params(0,1000,0);
    PIDcontroller pid(&params);
    setMicros(0);
    pid.set(0.0);
    advanceMicros(1000);
    FPASSERT(pid.update(0), 0.0);
    advanceMicros(1000);
    FPASSERT(pid.update(1), -1.0);
    advanceMicros(1000);
    FPASSERT(pid.update(0), -1.0);
    advanceMicros(1000);
    FPASSERT(pid.update(-2), 1.0);
    return true;
}

bool PIDderivative(){
    PIDparameters params(0,0,1.0/1000.0);
    PIDcontroller pid(&params);
    setMicros(0);
    pid.set(0.0);
    advanceMicros(1000);
    FPASSERT(pid.update(0), 0.0);
    advanceMicros(1000);
    FPASSERT(pid.update(1),-1.0);
    advanceMicros(1000);
    FPASSERT(pid.update(0), 1.0);
    advanceMicros(1000);
    FPASSERT(pid.update(-2),2.0);
    return true;
}

bool PIDintegralBound(){
    PIDparameters params(0,1000,0, -1.0, 1.0);
    PIDcontroller pid(&params);
    setMicros(0);
    pid.set(0.0);
    advanceMicros(1000);
    FPASSERT(pid.update(1), -1.0);
    advanceMicros(1000);
    FPASSERT(pid.update(1), -1.0);
    advanceMicros(1000);
    FPASSERT(pid.update(1), -1.0);
    advanceMicros(1000);
    FPASSERT(pid.update(0), -1.0);
    advanceMicros(1000);
    FPASSERT(pid.update(-1), 0.0);
    advanceMicros(1000);
    FPASSERT(pid.update(-1), 1.0);
    advanceMicros(1000);
    FPASSERT(pid.update(-1), 1.0);
    advanceMicros(1000);
    FPASSERT(pid.update(-1), 1.0);
    advanceMicros(1000);
    FPASSERT(pid.update(1), 0.0);
    return true;
}

bool PIDclearTest(){
    PIDparameters params(0,1000,0);
    PIDcontroller pid(&params);
    setMicros(0);
    pid.set(0.0);
    advanceMicros(1000);
    FPASSERT(pid.update(1), -1.0);
    advanceMicros(1000);
    FPASSERT(pid.update(0), -1.0);
    advanceMicros(1000);
    pid.clearAccumulator();
    FPASSERT(pid.update(0),  0.0);
    return true;
}

bool PIDtrainTest(){
    PIDparameters params(0,1000,0);
    PIDcontroller pid(&params);
    setMicros(0);
    pid.set(0.0);
    pid.train(-1);
    advanceMicros(1000);
    FPASSERT(pid.update(0), -1.0);
    pid.train(200);
    advanceMicros(1000);
    FPASSERT(pid.update(0), 200.0);
    pid.train(0);
    advanceMicros(1000);
    FPASSERT(pid.update(0),  0.0);
    return true;
}

bool PIDIllegalTrain(){
    PIDparameters params(0,1000,0, -1, 1);
    PIDcontroller pid(&params);
    setMicros(0);
    pid.set(0.0);
    pid.train(2);
    advanceMicros(1000);
    FPASSERT(pid.update(0), 1.0);
    pid.train(200);
    advanceMicros(1000);
    FPASSERT(pid.update(1), 0.0);
    pid.train(0);
    advanceMicros(1000);
    FPASSERT(pid.update(0),  0.0);
    return true;
}

bool PIDretuneTest(){
    PIDparameters params(1,0,0);
    PIDcontroller pid(&params);
    setMicros(0);
    pid.set(0.0);
    advanceMicros(1000);
    FPASSERT(pid.update(0), 0.0);
    advanceMicros(1000);
    FPASSERT(pid.update(1),-1.0);
    PIDparameters nP(10,0,0);
    pid.tune(&nP);
    advanceMicros(1000);
    FPASSERT(pid.update(2),-20.0);
    advanceMicros(1000);
    FPASSERT(pid.update(-100), 1000.0);
    return true;
}

bool PIDstopTest(){
    PIDparameters params(0,1000,0);
    PIDcontroller pid(&params);
    setMicros(0);
    pid.set(0.0);
    advanceMicros(1000);
    FPASSERT(pid.update(1), -1.0);
    pid.stop();
    advanceMicros(1000);
    FPASSERT(pid.update(1), 0.0);
    pid.set(1);//restarts on set
    advanceMicros(1000);
    FPASSERT(pid.update(0), 1.0);
    return true;
}

bool PIDtestNaN(){
    PIDparameters params(1,1,1);
    PIDcontroller pid(&params);
    setMicros(0);

    pid.set(0.0);
    advanceMicros(1000);
    float output = pid.update(NAN);
    FPASSERT(output, 0.0);
    advanceMicros(1000);
    output = pid.update(INFINITY);
    FPASSERT(output, 0.0);
    advanceMicros(1000);
    output = pid.update(-INFINITY);
    FPASSERT(output, 0.0);
    advanceMicros(1000);
    output = pid.update(1.0);
    ASSERT(output != 0.0);
    return true;
}

int main(){
    TEST(PIDporp            );
    TEST(PIDintegral        );
    TEST(PIDderivative      );
    TEST(PIDintegralBound   );
    TEST(PIDclearTest       );
    TEST(PIDtrainTest       );
    TEST(PIDIllegalTrain    );
    TEST(PIDretuneTest      );
    TEST(PIDstopTest        );
    TEST(PIDtestNaN         );
}
