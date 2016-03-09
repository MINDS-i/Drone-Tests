# Drone-Tests
Holds tests for MINDS-I-Drone project

to run tests, use "scons -s"; to rebuild, use "scons -c" first; to build
and run a single test using "scons only=filename"

An avr emulator and test runner based on the simavr project gets compiled to
    "testRunner"
* *.cpp files in "src" are compiled for avr and should include a main function
* "testHooks.h" contains functions for communicating with the test runner
* DroneLibs is included by default, but not linked
* adding //TESTING "search" to a test cpp file will cause any code matching
        the search in the drone libraries source directory to be complied and
        linked into that test's elf file

relative source paths and installation directories can be modified in
    the SConstruct file's leading variables
