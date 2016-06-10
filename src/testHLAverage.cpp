#include "testHooks.h"
#include "util/HLAverage.h"

bool testInitialValue(){
	HLA a(0.5, 10.0);
	FPASSERT(a.get(), 10.0);
	return true;
}

bool testSet(){
	HLA a(0.5, 10.0);
	a.set(-1.0);
	FPASSERT(a.get(), -1.0);
	return true;
}

bool testMillis(){
	HLA a(0.5, 0.0);
	a.update(0.0);
	advanceMillis(100);
	FPASSERT(a.millisSinceUpdate(), 100.0);
	return true;
}

bool testMicros(){
	HLA a(0.5, 0.0);
	a.update(0.0);
	advanceMicros(100);
	FPASSERT(a.microsSinceUpdate(), 100.0);
	return true;
}

bool resetTime(){
	HLA a(0.5, 0.0);
	a.update(0.0);
	advanceMicros(100);
	a.resetTime();
	FPASSERT(a.microsSinceUpdate(), 0.0);
	return true;
}

bool halfLife(){
	HLA one(1.0, 0.0);
	HLA ten(10.0, 0.0);
	advanceMicros(1000);
	one.update(1.0);
	advanceMicros(9000);
	ten.update(1.0);
	FPASSERT(one.get(), 0.5);
	FPASSERT(ten.get(), 0.5);
	return true;
}

bool hundredPassAccuracy(){
	HLA one(1.0, 0.0);
	for(int i=0; i<100; i++){
		advanceMicros(10);
		one.update(1.0);
	}
	FPASSERTC(one.get(), 0.5, 0.00001);
}

bool longHalfLife(){
	HLA one(1000000.0, 0.0);
	advanceMicros(1000000*1000);
	one.update(1.0);
	FPASSERT(one.get(), 0.5);
	return true;
}

bool shortHalfLife(){
	HLA one(0.001, 0.0);
	advanceMicros(1);
	one.update(1.0);
	FPASSERT(one.get(), 0.5);
}

HLA testHLA(0.5, 10.0);
void updateCalculation(){
	testHLA.update(0.0);
}

int main(){
	TEST(testInitialValue);
	TEST(testSet);
	TEST(testMillis);
	TEST(testMicros);
	TEST(resetTime);
	TEST(halfLife);
	TEST(longHalfLife);
	TEST(shortHalfLife);
	TEST(hundredPassAccuracy);
	BENCHMARK(updateCalculation);
}
