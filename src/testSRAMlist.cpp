#include <avr/io.h>
#include <util/delay.h>
#include <avr/sleep.h>
#include <stdio.h>

#include "testHooks.h"

#include "storage/List.h"
#include "storage/SRAMlist.h"

bool size(void){
    SRAMlist<int> l(20);
    ASSERT(l.size() == 0);
    for(int i=0; i<10; i++) l.add(i);
    ASSERT(l.size() == 10);
    return true;
}
bool maxSize(void){
    SRAMlist<int> l(20);
    ASSERT(l.maxSize() == 20);
    return true;
}
bool add(void){
    SRAMlist<int> l(20);
    for(int i=9; i>=0; i--) l.add(i);
    for(int i=0; i<10; i++) ASSERT(l.get(i) == i);
    return true;
}
bool insert(void){
    SRAMlist<int> l(20);
    for(int i=0; i<10; i++) l.add(4);
    l.add(5, 5);
    ASSERT(l.get(4) == 4);
    ASSERT(l.get(5) == 5);
    ASSERT(l.get(6) == 4);
    return true;
}
bool addTooMuch(void){
    SRAMlist<int> l(20);
    for(int i=0; i<20; i++) l.add(i);
    ASSERT(l.size() == 20);
    ASSERT(l.add(2) == false);
    return true;
}
bool pushTop(void){
    SRAMlist<int> l(20);
    for(int i=9; i>=0; i--) l.pushTop(i);
    for(int i=0; i<10; i++) ASSERT(l.get(i) == i);
    return true;
}
bool pushBottom(void){
    SRAMlist<int> l(20);
    for(int i=0; i<10; i++) l.pushBottom(i);
    for(int i=0; i<10; i++) ASSERT(l.get(i) == i);
    return true;
}
bool set(void){
    SRAMlist<int> l(20);
    for(int i=0; i<10; i++) l.pushBottom(i);
    l.set(5, 10);
    ASSERT(l.get(5) == 10);
    return true;
}
bool remove(void){
    SRAMlist<int> l(20);
    for(int i=0; i<10; i++) l.pushBottom(i);
    ASSERT(l.remove(5) == true);
    ASSERT(l.get(4) == 4);
    ASSERT(l.get(5) == 6);
    ASSERT(l.size() == 9);
    return true;
}
bool removeEmpty(void){
    SRAMlist<int> l(20);
    ASSERT(l.remove(3) == false);
    ASSERT(l.size() == 0);
    return true;
}
bool popTop(void){
    SRAMlist<int> l(20);
    for(int i=0; i<10; i++) l.pushBottom(i);
    for(int i=0; i<10; i++) ASSERT(l.popTop() == i);
    ASSERT(l.size() == 0);
    return true;
}
bool popBottom(void){
    SRAMlist<int> l(20);
    for(int i=0; i<10; i++) l.pushTop(i);
    for(int i=0; i<10; i++) ASSERT(l.popBottom() == i);
    ASSERT(l.size() == 0);
    return true;
}
bool clear(void){
    SRAMlist<int> l(20);
    for(int i=0; i<10; i++) l.pushTop(i);
    l.clear();
    ASSERT(l.size() == 0);
    return true;
}
bool stressTest(void){
    SRAMlist<int> l(20);
    for(int i=0; i<40; i++) l.remove(i);
    for(int i=0; i<40; i++) l.add(i);
    for(int i=0; i<40; i++) l.popBottom();
    for(int i=0; i<40; i++) l.pushTop(i);
    for(int i=0; i<40; i++) l.popTop();
    for(int i=0; i<40; i++) l.pushBottom(i);
    for(int i=0; i<40; i++) l.remove(0);
    ASSERT(l.size() == 0);
    ASSERT(l.add(1) == true);
    ASSERT(l.get(0) == 1);
    return true;
}

int main(void){
    TEST(size       );
    TEST(maxSize    );
    TEST(add        );
    TEST(insert     );
    TEST(addTooMuch );
    TEST(pushTop    );
    TEST(pushBottom );
    TEST(set        );
    TEST(remove     );
    TEST(removeEmpty);
    TEST(popTop     );
    TEST(popBottom  );
    TEST(clear      );
    TEST(stressTest );
    return 0;
}
