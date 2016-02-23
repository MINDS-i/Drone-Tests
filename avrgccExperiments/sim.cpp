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

using namespace std;

elf_firmware_t f;
avr_t * avr;

const avr_io_addr_t PORTB = 0x25;
const avr_io_addr_t OCDR  = 0x51;

const avr_io_addr_t DBGS  = 0x58;
const avr_io_addr_t DBGD  = 0x59;

class Test {
public:
    uint32_t start;
    uint32_t end;
    stringstream name;
    stringstream results;
    Test(): start(0), end(0), name(), results() {
    }
    void describe(ostream& out){
        out << "Completed " << name.str() << " in " << (end-start) << " cycles:" << endl;
        out << "\t" << results.str() << endl;
    }
};

const uint8_t TEST_START  = 3;
const uint8_t CLOCK_START = 2;
const uint8_t CLOCK_END   = 1;
const uint8_t TEST_END    = 0;

Test* test = new Test();

void sig_callback(struct avr_t * avr, avr_io_addr_t r, uint8_t v, void * param){
    avr->data[r] = v;
    //cerr << "sig write at cycle " << avr->cycle << " to " << (int)v << endl;

    switch(v){
        case TEST_START:
            delete test;
            test = new Test();
            break;
        case CLOCK_START:
            test->start = avr->cycle;
            break;
        case CLOCK_END:
            test->end = avr->cycle;
            break;
        case TEST_END:
            test->describe(cout);
            break;
    }
}

void data_callback(struct avr_t * avr, avr_io_addr_t r, uint8_t v, void * param){
    avr->data[r] = v;
    //cerr << "received " << (char) v << endl;

    if(test != NULL) {
        if(test->end != 0) test->results << (char) v;
        else test->name << (char) v;
    }
}

int main(int argc, char const *argv[]) {
    if(argc != 2){
        cerr << "Provide firmware as command argument" << endl;
        exit(1);
    }

    elf_read_firmware(argv[1], &f);
    avr = avr_make_mcu_by_name("atmega2560");
    if(!avr){
        cerr << "Failed to construct avr simulation" << endl;
        exit(1);
    }
    avr_init(avr);
    avr_load_firmware(avr, &f);

    avr_register_io_write(avr, DBGS, sig_callback, NULL);
    avr_register_io_write(avr, DBGD, data_callback, NULL);

    while (1) {
        int state = avr_run(avr);
        if ( state == cpu_Done || state == cpu_Crashed)
            break;
    }

    return 0;
}
