#ifndef MICROS_H
#define MICROS_H
#include <stdint.h>
#include <math.h>
#define constrain(amt,low,high) ((amt)<(low)?(low):((amt)>(high)?(high):(amt)))
typedef bool boolean;
uint32_t micros();
uint32_t millis();
void advanceMicros(uint32_t us);
void advanceMillis(uint32_t ms);
void setMicros(uint32_t us);
void setMillis(uint32_t ms);
#endif
