#include <iostream>
#include <vector>
#include <cstring>
#include <functional>
#include <cstdlib>
using namespace std;

#include "decoder/Decoder.h"
#include "test.h"

namespace DecoderTests{
    class TestIS : public InputStream {
        const char * pos;
    public:
        TestIS(const char * msg): pos(msg) {}
        int available() { return *pos != '\0'; }
        int read() { char c = *pos; pos++; return c; }
    };

    class TestReceiver : public Receiver {
        function<void(const char*, int)> handleFunc;
        function<int(char)> claimFunc;
    public:
        TestReceiver(function<int(char)> claimFunc,
                     function<void(const char*, int)> handleFunc):
            claimFunc(claimFunc), handleFunc(handleFunc) { }
        int claim(char data) { return claimFunc(data); }
        void handle(const char* buffer, int len) { handleFunc(buffer, len); }
    };
    class TestProtocol : public Protocol {
        function<bool(const char *, int)> check;
    public:
        TestProtocol(const char * header, int header_len,
                     const char * footer, int footer_len,
                     function<bool(const char *, int)> check):
            Protocol(header,header_len,footer,footer_len), check(check) {}
        bool checksum(const char* c, int len){ return check(c, len); }
    };
    class TestFactory {
    private:
        const char * header = "H";
        const char * footer = "F";
        function<bool(const char *, int)> check =
            [](const char* c, int i) { return true; };
        vector<Receiver*> receivers;
        TestProtocol protocol;
    public:
        TestFactory(): protocol(header, strlen(header), footer, strlen(footer), check){}
        ~TestFactory(){
            while(receivers.size() > 0){
                delete receivers.back();
                receivers.pop_back();
            }
        }
        void setHeader(const char * h){ header = h; }
        void setFooter(const char * f){ footer = f; }
        void setChecksum(function<bool(const char *, int)> c){ check = c; }
        void addReceiver(function<void(const char*, int)> handle, function<int(char)> claim){
            receivers.push_back(new TestReceiver(claim,handle));
        }
        void addReceiver(function<void(const char*, int)> handle){
            auto claimFunc = [](char c){return 256;};
            receivers.push_back(new TestReceiver(claimFunc,handle));
        }
        Decoder build(InputStream* is){
            protocol = TestProtocol(header, strlen(header),
                                    footer, strlen(footer),
                                    check);
            return Decoder(is, &protocol, receivers.data(), receivers.size());
        }
    };

    char readInput(){
        char c = getchar();
        if(c == '\n') return 0;
        return c;
    }

    bool matchRandomData() {
        TestFactory test;
        char result[128];
        char message[128];
        TestIS input("");
        int len;
        test.addReceiver([&](const char * c, int l){
            len = l;
            strncpy(result, c, l);
        });
        auto decoder = test.build(&input);

        for(int i=1; i<50; i++){
            sprintf(message, "H%*sF", i, ""); //write string with `i` spaces
            for(int j=0; j<i; j++) message[j+1] = 'a'+(rand()%26);
            input = TestIS(message);

            decoder.update();

            if(strncmp(message+1, result, i) != 0)
                return false;
        }
        return true;
    }
    bool checksumOverCorrectRange() {
        auto msg = "HabcdefgCF";
        TestIS input(msg);
        bool pass = false;
        TestFactory test;
        test.addReceiver([](const char * c, int l){});
        test.setChecksum([&](const char * c, int l){
            pass = (l == strlen(msg)-2) && (strncmp(msg+1, c, l) == 0);
            return true;
        });
        auto decoder = test.build(&input);

        decoder.update();

        return pass;
    }
    bool longHeader() {
        auto msg = "LONGHEADERabcdefCF";
        TestIS input(msg);
        bool pass = false;
        TestFactory test;
        test.setHeader("LONGHEADER");
        test.addReceiver([&](const char * c, int l){
            pass = (l == strlen(msg)-11) && (strncmp(msg+10, c, l) == 0);
        });
        auto decoder = test.build(&input);

        decoder.update();

        return pass;
    }
    bool longFooter() {
        auto msg = "HabcdefCLONGFOOTER";
        TestIS input(msg);
        bool pass = false;
        TestFactory test;
        test.setFooter("LONGFOOTER");
        test.addReceiver([&](const char * c, int l){
            pass = (l == strlen(msg)-11) && (strncmp(msg+1, c, l) == 0);
        });
        auto decoder = test.build(&input);

        decoder.update();

        return pass;
    }
    bool badPacketBefore() {
        auto msg = "HbadpacketFHpacketF";
        TestIS input(msg);
        auto count = 0;
        bool pass = false;
        TestFactory test;
        test.setChecksum([](const char * c, int l){ return *c=='p'; });
        test.addReceiver([&](const char * c, int l){
            count++;
            pass = (l == 6) && (strncmp("packet", c, l) == 0);
        });
        auto decoder = test.build(&input);

        decoder.update();

        return (pass && count == 1);
    }
    bool longGargbageStringBefore() {
        auto msg = "gargbagegarbagegarbageHpacketF";
        TestIS input(msg);
        bool pass = false;
        TestFactory test;
        test.addReceiver([&](const char * c, int l){
            pass = (l == 6) && (strncmp("packet", c, l) == 0);
        });
        auto decoder = test.build(&input);

        decoder.update();

        return pass;
    }
    bool multipleMatch() {
        auto msg = "HpacketFHpacketFHpacketF";
        TestIS input(msg);
        auto count = 0;
        bool pass = true;
        TestFactory test;
        test.addReceiver([&](const char * c, int l){
            count++;
            pass &= (l == 6) && (strncmp("packet", c, l) == 0);
        });
        auto decoder = test.build(&input);

        decoder.update();

        return (pass && count == 3);
    }
    bool multiplePacketTypes() {
        auto msg = "HaFHbF";
        TestIS input(msg);
        auto Acount = 0;
        auto Bcount = 0;
        TestFactory test;
        test.addReceiver([&](const char * c, int l){
                Acount++;
            },
            [](char c){return (c=='a')? 100 : -1;});
        test.addReceiver([&](const char * c, int l){
                Bcount++;
            },
            [](char c){return (c=='b')? 100 : -1;});
        auto decoder = test.build(&input);

        decoder.update();

        return (Acount == 1 && Bcount == 1);
    }
    bool decoyHeaderInData() {
        auto msg = "HHHHHHHHpacketF";
        TestIS input(msg);
        auto count = 0;
        bool pass = false;
        TestFactory test;
        test.setChecksum([](const char * c, int l){
            return (l == 6);
        });
        test.addReceiver([&](const char * c, int l){
            count++;
            pass = (l == 6) && (strncmp("packet", c, l) == 0);
        });
        auto decoder = test.build(&input);

        decoder.update();

        return (pass && count == 1);
    }
    bool decoyFooterInData() {
        auto msg = "HpacketFFFFFFFFF";
        TestIS input(msg);
        auto count = 0;
        bool pass = false;
        TestFactory test;
        test.setChecksum([](const char * c, int l){
            return (l == 6);
        });
        test.addReceiver([&](const char * c, int l){
            count++;
            pass = (l == 6) && (strncmp("packet", c, l) == 0);
        });
        auto decoder = test.build(&input);

        decoder.update();

        return (pass && count == 1);
    }
    bool dontClaimMessage() {
        auto msg = "HpacketFHpacketF";
        TestIS input(msg);
        auto count = 0;
        TestFactory test;
        test.addReceiver([&](const char * c, int l){
                count++;
            },
            [](char c){return -1;}); //don't claim any messages
        auto decoder = test.build(&input);

        decoder.update();

        return (count == 0);
    }
    bool badChecksum() {
        auto msg = "HpacketFHpacketF";
        TestIS input(msg);
        auto count = 0;
        TestFactory test;
        test.setChecksum([](const char * c, int l){
            return false;
        });
        test.addReceiver([&](const char * c, int l){
            count++;
        });
        auto decoder = test.build(&input);

        decoder.update();

        return (count == 0);
    }
    void runTests() {
        TEST(matchRandomData         );
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
}
