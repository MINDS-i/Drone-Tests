#include <avr/io.h>
#include <util/delay.h>
#include <avr/sleep.h>
#include <stdio.h>

#include "testHooks.h"

#include "storage/List.h"
#include "storage/EEPROMlist.h"
//TESTING "math/Waypoint.cpp"

bool size(void){
    EEPROMlist* l = EEPROMlist::getInstance();
    ASSERT(l->size() == 0);
    for(int i=0; i<10; i++) l->add(Waypoint(i,i));
    ASSERT(l->size() == 10);
    return true;
}
bool maxSize(void){
    printf("Starting test");
    EEPROMlist* l = EEPROMlist::getInstance();
    //ASSERT(l->maxSize() == 20);

    for(volatile int i=0; i<1000; i++) ;
    return true;
}
bool add(void){
    EEPROMlist* l = EEPROMlist::getInstance();
    for(int i=9; i>=0; i--) l->add(Waypoint(i,i));
    for(int i=0; i<10; i++) ASSERT(l->get(i).lat == i);
    return true;
}
bool insert(void){
    EEPROMlist* l = EEPROMlist::getInstance();
    for(int i=0; i<10; i++) l->add(Waypoint(4,4));
    l->add(5, Waypoint(5,5));
    ASSERT(l->get(4).lat == 4);
    ASSERT(l->get(5).lat == 5);
    ASSERT(l->get(6).lat == 4);
    return true;
}
bool addTooMuch(void){
    EEPROMlist* l = EEPROMlist::getInstance();
    for(int i=0; i<20; i++) l->add(Waypoint(i,i));
    ASSERT(l->size() == 20);
    ASSERT(l->add(Waypoint()) == false);
    return true;
}
bool pushTop(void){
    EEPROMlist* l = EEPROMlist::getInstance();
    for(int i=9; i>=0; i--) l->pushTop(Waypoint(i,i));
    for(int i=0; i<10; i++) ASSERT(l->get(i).lat == i);
    return true;
}
bool pushBottom(void){
    EEPROMlist* l = EEPROMlist::getInstance();
    for(int i=0; i<10; i++) l->pushBottom(Waypoint(i,i));
    for(int i=0; i<10; i++) ASSERT(l->get(i).lat == i);
    return true;
}
bool set(void){
    EEPROMlist* l = EEPROMlist::getInstance();
    for(int i=0; i<10; i++) l->pushBottom(Waypoint(i,i));
    l->set(5, Waypoint(10,10));
    ASSERT(l->get(5).lat == 10);
    return true;
}
bool remove(void){
    EEPROMlist* l = EEPROMlist::getInstance();
    for(int i=0; i<10; i++) l->pushBottom(Waypoint(i,i));
    l->remove(5);
    ASSERT(l->get(4).lat == 4);
    ASSERT(l->get(5).lat == 6);
    ASSERT(l->size() == 9);
    return true;
}
bool removeEmpty(void){
    EEPROMlist* l = EEPROMlist::getInstance();
    l->remove(3);
    ASSERT(l->size() == 0);
    return true;
}
bool popTop(void){
    EEPROMlist* l = EEPROMlist::getInstance();
    for(int i=0; i<10; i++) l->pushBottom(Waypoint(i,i));
    for(int i=0; i<10; i++) ASSERT(l->popTop().lat == i);
    ASSERT(l->size() == 0);
    return true;
}
bool popBottom(void){
    EEPROMlist* l = EEPROMlist::getInstance();
    for(int i=0; i<10; i++) l->pushTop(Waypoint(i,i));
    for(int i=0; i<10; i++) ASSERT(l->popBottom().lat == i);
    ASSERT(l->size() == 0);
    return true;
}
bool clear(void){
    EEPROMlist* l = EEPROMlist::getInstance();
    for(int i=0; i<10; i++) l->pushTop(Waypoint(i,i));
    l->clear();
    ASSERT(l->size() == 0);
    return true;
}
bool stressTest(void){
    EEPROMlist* l = EEPROMlist::getInstance();
    for(int i=0; i<40; i++) l->remove(i);
    for(int i=0; i<40; i++) l->add(Waypoint(i,i));
    for(int i=0; i<40; i++) l->popBottom();
    for(int i=0; i<40; i++) l->pushTop(Waypoint(i,i));
    for(int i=0; i<40; i++) l->popTop();
    for(int i=0; i<40; i++) l->pushBottom(Waypoint(i,i));
    for(int i=0; i<40; i++) l->remove(0);
    ASSERT(l->size() == 0);
    ASSERT(l->add(Waypoint(1,1)) == true);
    ASSERT(l->get(0).lat == 1);
    return true;
}

int main(void){
    sei();
//    TEST(size       );
    TEST(maxSize    );
//    TEST(add        );
/*    TEST(insert     );
    TEST(addTooMuch );
    TEST(pushTop    );*/
/*    TEST(pushBottom );
    TEST(set        );
    TEST(remove     );
    TEST(removeEmpty);
    TEST(popTop     );
    TEST(popBottom  );
    TEST(clear      );
    TEST(stressTest );*/
    cli();
    return 0;
}
