using namespace std;
#include "testHooks.h"
#include "util/circBuf.h"

typedef circBuf<char, 64> Buffer;

void load(const char* c, Buffer& b){
    while(*c != '\0'){
        b.add(*c);
        c++;
    }
}

bool testAdd() {
    Buffer buf;
    buf.add('a');
    CASSERT(buf.get(buf.start()), 'a');
    buf.add('b');
    CASSERT(buf.get(buf.start()), 'a');
    CASSERT(buf.get(buf.end()-1), 'b');
    buf.add('c');
    CASSERT(buf.get(buf.start()), 'a');
    CASSERT(buf.get(buf.end()-1), 'c');
    return true;
}
bool testArray() {
    Buffer buf;
    buf.add('a');
    CASSERT(buf[buf.start()], 'a');
    buf.add('b');
    CASSERT(buf[buf.start()], 'a');
    CASSERT(buf[buf.end()-1], 'b');
    buf.add('c');
    CASSERT(buf[buf.start()], 'a');
    CASSERT(buf[buf.end()-1], 'c');
    return true;
}
bool testRemove() {
    Buffer buf;
    load("abcdefghijklmnopqrstuvwxyz", buf);
    CASSERT(buf.get(buf.start()), 'a');
    buf.remove(0);
    CASSERT(buf.get(buf.start()), 'a');
    buf.remove(1);
    CASSERT(buf.get(buf.start()), 'b');
    buf.remove(1);
    CASSERT(buf.get(buf.start()), 'c');
    buf.remove(10);
    CASSERT(buf.get(buf.start()), 'm');
    buf.remove(1000);
    CASSERT(buf.size(), 0);
    return true;
}
bool testSize() {
    Buffer buf;
    CASSERT(buf.size(), 0);
    buf.add('a');
    CASSERT(buf.size(), 1);
    load("1234567890", buf);
    CASSERT(buf.size(), 11);
    buf.remove(5);
    CASSERT(buf.size(), 6);
    for(int i=0; i<1000; i++) buf.add('a');
    CASSERT(buf.size(), 64);
    return true;
}
bool testRemaining() {
    Buffer buf;
    CASSERT(buf.remaining(), 64);
    buf.add('a');
    CASSERT(buf.remaining(), 63);
    load("1234567890", buf);
    CASSERT(buf.remaining(), 53);
    buf.remove(7);
    CASSERT(buf.remaining(), 60);
    for(int i=0; i<1000; i++) buf.add('a');
    CASSERT(buf.remaining(), 0);
    return true;
}
bool testOverwrite() {
    Buffer buf;
    for(int i=0; i<64; i++) {
        buf.add('a');
    }
    for(int i=0; i<63; i++) {
        buf.add('b');
        CASSERT(buf.get(buf.start()), 'a');
    }
    buf.add('b');
    CASSERT(buf.get(buf.start()), 'b');
    return true;
}
int main() {
    TEST(testAdd      );
    TEST(testArray    );
    TEST(testRemove   );
    TEST(testSize     );
    TEST(testRemaining);
    TEST(testOverwrite);
}
