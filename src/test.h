#ifndef TESTS_H
#define TESTS_H

#include <iostream>
#include <math.h>

#define ASSERT(a) if(a) { \
	std::cout<< #a << " was false";\
	return false; \
}
#define CASSERT(a,b) { \
	if(!((a) == (b))) { \
		std::cout<< #a << "=" << a << "  !=  " << #b << " "; \
		return false; \
	} \
}
#define FPASSERT(a,b) { \
const float ans = a; \
const float chk = b; \
if(!fuzzyCompare(ans,chk)) { \
	std::cout<<#a<<" = "<<ans<<" != "<<chk<<" "; \
	return false; \
} \
}
#define FPASSERTC(a,b,c) { \
const float ans = a; \
const float chk = b; \
if(!fuzzyCompare(a,b,c)) { \
	std::cout<<#a<<" = "<<a<<" != "<<b<<" "; \
	return false; \
}\
}
#define TEST(a) test(&a, #a);
const int EXTRA_WIDTH = 32;

bool test(bool (*func)(void), std::string name){
	std::cout << "  " << name << ": ";
	for(int i=EXTRA_WIDTH - name.length(); i>0; i--) std::cout<<'-';
	std::cout << "> ";

	bool result = func();
	if(result){
		std::cout << "passed";
	} else {
		std::cout << "FAILED";
	}

	std::cout << std::endl;
	return result;
}
bool fuzzyCompare(float a, float b, float diff=0.0001){
	return (fabs(a-b) < diff);
}

float randF(float min, float max){
	static bool seededRandom = (srand(time(NULL)), true); //your welcome

	float rtn = std::rand();
	rtn /= float(RAND_MAX);
	rtn *= (max - min);
	rtn += min;
	return rtn;
}
#endif
