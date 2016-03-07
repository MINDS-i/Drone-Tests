#include <iostream>
#include <libgen.h>
#include <pthread.h>
#include <sstream>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>

#include "avr_ioport.h"
#include "avr_twi.h"
#include "sim_avr.h"
#include "sim_elf.h"
#include "sim_gdb.h"
#include "sim_vcd_file.h"

#include "testHooksCommon.h"

using namespace std;

elf_firmware_t f;
avr_t * avr;

const int LINE_LENGTH = 40;

namespace Colors{
    typedef const char * cstr;
    cstr Black       = "\033[0;30m";
    cstr Blue        = "\033[0;34m";
    cstr Orange      = "\033[0;33m";
    cstr Cyan        = "\033[0;36m";
    cstr DarkGray    = "\033[1;30m";
    cstr Green       = "\033[0;32m";
    cstr LightBlue   = "\033[1;34m";
    cstr LightCyan   = "\033[1;36m";
    cstr LightGray   = "\033[0;37m";
    cstr LightGreen  = "\033[1;32m";
    cstr LightPurple = "\033[1;35m";
    cstr LightRed    = "\033[1;31m";
    cstr NoColor     = "\033[0m";
    cstr Purple      = "\033[0;35m";
    cstr Red         = "\033[0;31m";
    cstr White       = "\033[1;37m";
    cstr Yellow      = "\033[1;33m";
}


class Test {
public:
    uint32_t start;
    uint32_t end;
    stringstream name;
    stringstream debug;
    bool nameDone;
    bool passed;
    Test(): start(0), end(0), name(), debug(), nameDone(false), passed(false) {
    }
    void clockStart(uint32_t clock){
        start = clock;
        nameDone = true;
    }
    void finish(uint32_t clock, bool pass) {
        end = clock;
        passed = pass;
        describe(cout);
    }
    void receive(uint8_t v) {
        if(nameDone) debug << (char) v;
        else name << (char) v;
    }
private:
    void describe(ostream& out){

        out << ((passed)? Colors::LightBlue : Colors::LightRed); {
            out << name.str() << " ";
        }

        out << Colors::DarkGray; {
            for(int i=name.str().length()+3; i<LINE_LENGTH; i++)
                out << "-";
            out << "> ";
        }

        out << ((passed)? Colors::LightBlue : Colors::LightRed); {
            out << ((passed)? "passed" : "FAILED") << " ";
        }

        out << Colors::Blue; {
            out << "(" << (end-start) << " cycles)";
            out << endl;
        }

        out << Colors::NoColor;

        //Echo the debugging info we received, but tabbed in one level
        if(debug.str().length() > 0){
            out << "\t";
            for(auto c : debug.str()){
                if(c == '\n') out << endl << "\t";
                else out << c;
            }
            out << endl;
        }
    }
};

Test* test = new Test();

void data_callback(struct avr_t * avr, avr_io_addr_t r, uint8_t v, void * param){
    avr->data[r] = v;

    test->receive(v);
}

void sig_callback(struct avr_t * avr, avr_io_addr_t r, uint8_t v, void * param){
    avr->data[r] = v;

    switch(v){
        case NAME_START:
            delete test;
            test = new Test();
            break;
        case CLOCK_START:
            test->clockStart(avr->cycle);
            break;
        case TEST_PASS:
            test->finish(avr->cycle, true);
            break;
        case TEST_FAIL:
            test->finish(avr->cycle, false);
            break;
    }
}

int main(int argc, char const *argv[]) {
    if(argc != 2){
        cerr << "Provide firmware as command argument" << endl;
        exit(1);
    }

    cout << Colors::LightPurple; {
        cout << "Running tests from " << argv[1] << endl;
        elf_read_firmware(argv[1], &f);
        avr = avr_make_mcu_by_name("atmega2560");
    } cout << Colors::NoColor;

    if(!avr){
        cerr << "Failed to construct avr simulation" << endl;
        exit(1);
    }
    avr_init(avr);
    avr_load_firmware(avr, &f);

    avr_register_io_write(avr, SIGNAL_IO_NUM, sig_callback, NULL);
    avr_register_io_write(avr, DATA_IO_NUM, data_callback, NULL);

    while (1) {
        int state = avr_run(avr);
        if ( state == cpu_Done || state == cpu_Crashed)
            break;
    }

    return 0;
}
