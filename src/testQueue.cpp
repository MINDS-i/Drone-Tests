#include <avr/io.h>
#include <util/delay.h>
#include <avr/sleep.h>
#include <stdio.h>

#include "testHooks.h"

#include "storage/queue.h"

bool dataOrder(void) {
    int buffer[10];
    SimpleQueue<int> queue(buffer, 10);
    for(int i=0; i<10; i++) queue.push(i);
    for(int i=0; i<10; i++) {
        int result = queue.pop();
        if(result != i){
            printf("Queue expected %i, popped %i\n", i, result);
            return false;
        }
    }
    return true;
}
bool dataOrderCleared(void) {
    int buffer[10];
    SimpleQueue<int> queue(buffer, 10);
    for(int i=0; i<5; i++){
        for(int j=0; j<i; j++) queue.push(i*j);
        for(int j=0; j<i; j++) queue.pop();
    }
    ASSERT(queue.isEmpty());
    for(int i=0; i<10; i++) queue.push(i);
    for(int i=0; i<10; i++) {
        int result = queue.pop();
        if(result != i){
            printf("Queue expected %i, popped %i\n", i, result);
            return false;
        }
    }
    return true;
}
bool tooManyPush(void) {
    int buffer[10];
    SimpleQueue<int> queue(buffer, 10);
    for(int i=0; i<10; i++) queue.push(2);
    queue.push(10);
    return queue.pop() == 2;
}
bool tooManyPop(void) {
    int buffer[10];
    SimpleQueue<int> queue(buffer, 10);
    for(int i=0; i<10; i++) queue.pop();
    queue.push(10);
    return queue.pop() == 10;
}
bool pushTrueWhenEmpty(void) {
    int buffer[10];
    SimpleQueue<int> queue(buffer, 10);
    return queue.push(0);
}
bool pushTrueWhenCleared(void) {
    int buffer[10];
    SimpleQueue<int> queue(buffer, 10);
    for(int i=0; i<10; i++) queue.push(i);
    queue.pop();
    return queue.push(0);
}
bool pushFalseWhenFull(void) {
    int buffer[10];
    SimpleQueue<int> queue(buffer, 10);
    for(int i=0; i<10; i++) queue.push(i);
    return !queue.push(0);
}
bool isFull(void) {
    int buffer[10];
    SimpleQueue<int> queue(buffer, 10);
    for(int i=0; i<10; i++) queue.push(2);
    return queue.isFull();
}
bool isEmpty(void) {
    int buffer[10];
    SimpleQueue<int> queue(buffer, 10);
    for(int i=0; i<10; i++) queue.push(i);
    for(int i=0; i<10; i++) queue.pop();
    return queue.isEmpty();
}

int main(void){
    TEST(dataOrder);
    TEST(dataOrderCleared);
    TEST(tooManyPush);
    TEST(tooManyPop);
    TEST(pushTrueWhenEmpty);
    TEST(pushFalseWhenFull);
    TEST(isFull);
    TEST(isEmpty);
    return 0;
}
