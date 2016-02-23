#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <libgen.h>
#include <pthread.h>

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

void write_callback(struct avr_t * avr, avr_io_addr_t r, uint8_t v, void * param){
    avr->data[r] = v;
    cerr << "pin write at cycle " << avr->cycle << " to " << (int)v << endl;
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

    avr_register_io_write(
        avr,
        DBGS,
        write_callback,
        NULL);

    while (1) {
        int state = avr_run(avr);
        if ( state == cpu_Done || state == cpu_Crashed)
            break;
    }

    return 0;
}
