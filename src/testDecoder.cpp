#include "string.h"
#include "stdlib.h"

#include "testHooks.h"
#include "TestStreams.h"
#include "decoder/Decoder.h"
//TESTING "decoder/Decoder.cpp"

bool alwaysTrue(const char *c, int l){ return true; }
int rtn256(char c, void* v){ return 256; }

class TestProtocol : public Protocol {
    bool (*check)(const char *, int);
public:
    TestProtocol(const char * header, int header_len,
                 const char * footer, int footer_len,
                 bool (*check)(const char *, int)):
        Protocol(header,header_len,footer,footer_len), check(check) {}
    bool checksum(const char* c, int len){ return check(c, len); }
};
class TestReceiver : public Receiver {
    void (*handleFunc)(const char*, int, void*);
    void* hctx;
    int (*claimFunc)(char, void*);
    void* cctx;
public:
    TestReceiver(void (*handleFunc)(const char*, int, void*), void* hctx):
        claimFunc(&rtn256), cctx(NULL), handleFunc(handleFunc), hctx(hctx) { }
    TestReceiver(void (*handleFunc)(const char*, int, void*), void* hctx,
                 int (*claimFunc)(char, void*)              , void* cctx ):
        claimFunc(claimFunc), cctx(cctx),
        handleFunc(handleFunc), hctx(hctx) { }
    int claim(char data) { return claimFunc(data, cctx); }
    void handle(const char* buffer, int len) { handleFunc(buffer, len, hctx); }
};
class TestFactory {
private:
    const char * header = "H";
    const char * footer = "F";
    bool (*check)(const char *, int) =
        [](const char* c, int i) { return true; };
    Receiver* receivers[10];
    int       numReceivers = 0;
    TestProtocol protocol;
public:
    TestFactory(): protocol(header, strlen(header), footer, strlen(footer), check){}
    ~TestFactory(){}
    void setHeader(const char * h){ header = h; }
    void setFooter(const char * f){ footer = f; }
    void setChecksum(bool (*checksum)(const char *, int)){ check = checksum; }
    void addReceiver(Receiver* rec){
        receivers[numReceivers] = rec;
        numReceivers++;
    }
    Decoder build(Stream& is){
        protocol = TestProtocol(header, strlen(header),
                                footer, strlen(footer),
                                check);
        return Decoder(is, &protocol, receivers, numReceivers);
    }
};

char readInput(){
    char c = getchar();
    if(c == '\n') return 0;
    return c;
}

bool checksumOverCorrectRange() {
    constexpr const char * msg = "HabcdefgCF";
    StringStream input(msg);
    static bool pass = false;
    TestFactory test;
    TestReceiver rec([](const char * c, int l, void* null){
    }, NULL);
    test.setChecksum([](const char * c, int l){
        pass = (strncmp(msg+1, c, l) == 0);
        return pass;
    });
    test.addReceiver((Receiver*)&rec);
    auto decoder = test.build(input);

    decoder.update();

    return pass;
}
bool longHeader() {
    constexpr const char * msg = "LONGHEADERabcdefCF";
    StringStream input(msg);
    bool pass = false;

    TestFactory test;
    test.setHeader("LONGHEADER");
    TestReceiver rec([](const char * c, int l, void* pass){
        (*((bool*)pass)) = (strncmp(msg+10, c, l) == 0);
    }, (void*)&pass);
    test.addReceiver((Receiver*)&rec);
    auto decoder = test.build(input);

    decoder.update();

    return pass;
}
bool longFooter() {
    constexpr const char * msg = "HabcdefCLONGFOOTER";
    StringStream input(msg);
    bool pass = false;
    TestFactory test;
    test.setFooter("LONGFOOTER");
    TestReceiver rec([](const char * c, int l, void* pass){
        (*((bool*)pass)) = (strncmp(msg+1, c, l) == 0);
    }, &pass);
    test.addReceiver((Receiver*)&rec);
    auto decoder = test.build(input);

    decoder.update();

    return pass;
}
bool badPacketBefore() {
    constexpr const char * msg = "HbadpacketFHpacketF";
    StringStream input(msg);
    int count = 0;
    TestFactory test;
    test.setChecksum([](const char * c, int l){ return *c=='p'; });

    TestReceiver rec([](const char * c, int l, void* count){
        if((strncmp("packet", c, l) == 0)) (*((int*)count))++;
    }, &count);
    test.addReceiver((Receiver*)&rec);
    auto decoder = test.build(input);

    decoder.update();

    return count == 1;
}
bool longGargbageStringBefore() {
    constexpr const char * msg = "gargbagegarbagegarbageHpacketF";
    StringStream input(msg);
    bool pass = false;
    TestFactory test;

    TestReceiver rec([&](const char * c, int l, void* pass){
        (*((bool*)pass)) = (strncmp("packet", c, l) == 0);
    }, &pass);
    test.addReceiver((Receiver*)&rec);
    auto decoder = test.build(input);

    decoder.update();

    return pass;
}
bool multipleMatch() {
    constexpr const char * msg = "HpacketFHpacketFHpacketF";
    StringStream input(msg);
    int count = 0;
    bool pass = true;
    TestFactory test;

    TestReceiver rec([](const char * c, int l, void* count){
        (*(int*)count)++;
        if(!((strncmp("packet", c, l) == 0))){
            (*(int*)count) = -100;
        }
    }, &count);
    test.addReceiver((Receiver*)&rec);
    auto decoder = test.build(input);

    decoder.update();

    return (count == 3);
}
bool multiplePacketTypes() {
    constexpr const char * msg = "HaFHbF";
    StringStream input(msg);
    auto Acount = 0;
    auto Bcount = 0;
    TestFactory test;

    TestReceiver aRec([&](const char * c, int l, void* count){
            (*((int*)count))++;
        }, &Acount,
        [](char c, void* n){return (c=='a')? 100 : -1;}, NULL);
    TestReceiver bRec([&](const char * c, int l, void* count){
            (*((int*)count))++;
        }, &Bcount,
        [](char c, void* n){return (c=='b')? 100 : -1;}, NULL);

    test.addReceiver((Receiver*)&aRec);
    test.addReceiver((Receiver*)&bRec);

    auto decoder = test.build(input);

    decoder.update();

    return (Acount == 1 && Bcount == 1);
}
bool decoyHeaderInData() {
    constexpr const char * msg = "HHHHHHHHpacketF";
    StringStream input(msg);
    int count = 0;
    TestFactory test;
    test.setChecksum([](const char * c, int l){
        return (l == 6);
    });

    TestReceiver rec([](const char * c, int l, void* count){
        (*((int*)count))++;
        if( !(strncmp("packet", c, l) == 0) ){
            *((int*)count) = -100;
        }
    }, &count);
    test.addReceiver((Receiver*)&rec);

    auto decoder = test.build(input);

    decoder.update();

    return count == 1;
}
bool decoyFooterInData() {
    constexpr const char * msg = "HpacketFFFFFFFFF";
    StringStream input(msg);
    int count = 0;
    TestFactory test;
    test.setChecksum([](const char * c, int l){
        return (l == 6);
    });
    TestReceiver rec([](const char * c, int l, void* count){
        (*((int*)count))++;
        if(! (strncmp("packet", c, l) == 0) ){
            *((int*)count) = -100;
        }
    }, &count);
    test.addReceiver((Receiver*)&rec);
    auto decoder = test.build(input);

    decoder.update();

    return (count == 1);
}
bool dontClaimMessage() {
    constexpr const char * msg = "HpacketFHpacketF";
    StringStream input(msg);
    int count = 0;
    TestFactory test;

    TestReceiver rec([](const char * c, int l, void* count){
                     (*((int*)count))++;
                 }, &count,
                 [](char c, void * n){
                    return 0;
                 }, NULL
                );
    test.addReceiver((Receiver*)&rec);
    auto decoder = test.build(input);

    decoder.update();

    return count == 0;
}
bool badChecksum() {
    constexpr const char * msg = "HpacketFHpacketF";
    StringStream input(msg);
    int count = 0;
    TestFactory test;
    test.setChecksum([](const char * c, int l){
        return false;
    });
    TestReceiver rec([](const char * c, int l, void* count){
        (*((int*)count))++;
    }, &count);
    test.addReceiver((Receiver*)&rec);
    auto decoder = test.build(input);

    decoder.update();

    return (count == 0);
}
int main() {
    TEST(checksumOverCorrectRange);
    TEST(longHeader              );
    TEST(longFooter              );
    TEST(badPacketBefore         );
    TEST(longGargbageStringBefore);
    TEST(multipleMatch           );
    TEST(multiplePacketTypes     );
    TEST(decoyHeaderInData       );
    TEST(decoyFooterInData       );
    TEST(dontClaimMessage        );
    TEST(badChecksum             );
}
