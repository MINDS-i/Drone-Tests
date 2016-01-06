#include "Arduino.h"

uint32_t _falseTime;
uint32_t micros(){
	return _falseTime;
}
uint32_t millis(){
	return (_falseTime/1000UL);
}
void advanceMicros(uint32_t us){
	_falseTime += us;
}
void advanceMillis(uint32_t ms){
	_falseTime += ms*1000;
}
void setMicros(uint32_t us){
	_falseTime = us;
}
void setMillis(uint32_t ms){
	_falseTime = ms*1000;
}
