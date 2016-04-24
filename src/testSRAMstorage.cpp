#include <avr/io.h>
#include <util/delay.h>
#include <avr/sleep.h>
#include <stdio.h>

#include "testHooks.h"
#include "storage/Storage.h"
#include "storage/SRAMstorage.h"

void writeTrue(bool* b){
    *b = true;
}

bool updateRecord(void) {
    SRAMstorage<int, 20> s;
    s.updateRecord(3, 5);
    ASSERT(s.getRecord(3) == 5);
    return true;
}
bool attachCallback(void) {
    SRAMstorage<bool*, 20> s;
    bool success = false;
    s.updateRecord(0, &success);
    s.attachCallback(0, writeTrue);
    return success;
}
bool updateCallback(void) {
    SRAMstorage<bool*, 20> s;
    bool success;
    s.updateRecord(19, &success);
    s.attachCallback(19, writeTrue);
    success = false;
    s.updateRecord(19, &success);
    return success;
}

int main(void){
    TEST(updateRecord);
    TEST(attachCallback);
    TEST(updateCallback);
    return 0;
}
