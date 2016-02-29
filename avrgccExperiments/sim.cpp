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
        out << name.str() << " ";
        for(int i=name.str().length()+3; i<LINE_LENGTH; i++)
            cout << "-";
        cout << "> " << ((passed)? "passed" : "FAILED") << " ";
        cout << "(" << (end-start) << " cycles)";
        cout << endl;

        if(debug.str().length() > 0){
            cout << "\t";
            for(auto c : debug.str()){
                if(c == '\n') cout << endl << "\t";
                else cout << c;
            }
            cout << endl;
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

    cout << "Running tests from " << argv[1] << endl;
    elf_read_firmware(argv[1], &f);
    avr = avr_make_mcu_by_name("atmega2560");
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
