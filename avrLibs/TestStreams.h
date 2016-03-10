#pragma once

#include "Arduino.h"
/**
 * Implementation of Stream that returns an input string char by char
 * It reports writes to the console
 */
class StringStream : public Stream {
private:
    const char * str;
public:
    StringStream(const char * str): str(str) {}
    /** reset the stream to point to a new string */
    void reset(const char * newstr){
        str = newstr;
    }
    /** Number of available chars in the stream */
    int available(){
        return strlen(str);
    }
    /** Get the next char from the stream, removing it, -1 for no data */
    int read(){
        if(*str == '\0') return -1;
        int next = *str;
        str++;
        return next;
    }
    /** Get the next char from the stream without removing it */
    int peek(){
        if(*str == '\0') return -1;
        int next = *str;
        return next;
    }
    /** Wait for any outgoing data to finish being transmitted */
    void flush(){ }
    /** write a byte to output */
    size_t write(uint8_t input){
        printf("%c", input);
    }
    /** write a series of bytes to output */
    size_t write(const uint8_t *buffer, size_t s){
        for(int i=0; i<s; i++) write(buffer[i]);
    }
};

/**
 * implementation of stream that reports everything happening to it
 * Suitable for overridding to instrument a particular method
 */
class MockStream : public Stream {
public:
    MockStream(){}
    /** Number of available chars in the stream */
    int available(){
        printf("MockStream: call to available\n");
        return 0;
    }
    /** Get the next char from the stream, removing it */
    int read(){
        printf("MockStream: call to read\n");
        return -1;
    }
    /** Get the next char from the stream without removing it */
    int peek(){
        printf("MockStream: call to peek\n");
        return -1;
    }
    /** Wait for any outgoing data to finish being transmitted */
    void flush(){
        printf("MockStream: call to flush\n");
    }
    /** write a byte to output */
    size_t write(uint8_t input){
        printf("MockStream: call to write with \'%c\'(%02X)\n", input, input);
    }
    /** write a series of bytes to output */
    size_t write(const uint8_t *buffer, size_t s){
        printf("MockStream: call to write with %i len buffer: \n", s);
        do {
            int chomp = min(8, s);
            for(int i=0; i<chomp; i++) printf("%c", buffer[i]);
            for(int i=0; i<12-chomp; i++) printf(" ");
            for(int i=0; i<chomp; i++) printf("%02X ", buffer[i]);
            printf("\n");
            buffer += chomp;
            s -= chomp;
        } while(s > 0);
    }
};
