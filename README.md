# Zuse Elektra emulator
The Zuse Elektra is a system family from Zuse Gerätebau implementing the EZER/OSER architecture. Released in 1964, it has diversified into a line of mainframes, superminis, workstations and servers. The English Electric Electra is a licensed clone.

This emulator emulates a 32-bit Zuse Elektra, configurable via a configuration file specifying:
* the amount of RAM in 4K pages;
* the units, in the format *k t [ p ]*, where
* * *k* is the type of unit (**r** for processing, **d** for channel);
* * *t* is the interrupt port address, in hex;
* * *p* are the parameters for each unit.

## Status
* Memory is verified; all specified features work, but some implementation features need to be frozen (such as cache sizing and replacement algorithms).
* Channel units are in progress; tests and devices still need to be written.
* Processing units are in progress; tests need to be written and some aspects need to be frozen, such as cache sizes.

## Building
### Systems
#### Make
Do set CXX in the make command:

`make CXX=clang++ prf/durchgangeinheit  # To compile the channel test program with clang++`

If your CXX is neither g++ nor clang++, you may pass CXXFLAGS to tune appropriately:

`make CXX=icc CXXFLAGS='-qopenmp -debug=parallel' # To compile the channel test program with the Intel C++ compiler, for example`

### Dependencies
The emulator is written in C++11, but a C++20-compatible compiler is recommended for increased performance.

### Targets
* release: release builds of the emulator and devices;
* debug: debug builds of the emulator and devices;
* prufungen: all tests;
* prf/*: the individual tests.

### Testing
Tests in prf/ are intended to be run in this order:
* speicher (tests memory);
* durchgangeinheit (tests channels and some essential devices) (in progress);
* recheneinheit (tests CPUs) (in progress).

The test is successful if it returns normally, i.e. doesn't call terminate after throwing an exception or doesn't segfault. The tests can be run under gdb for debugging.

## Documentation
The principal documentation are the PDFs in doc/:
* 32.pdf: The EZER-32 specification, in English;
* de\_32.pdf / ae\_32.pdf: The original texts of the EZER-32 specification, in German and Ard English;
* 64.pdf: The EZER-64 specification, in English;
* de\_64.pdf / ae\_32.pdf: The original texts of the EZER-64 specification, in German and Ard English.

## Further information
Visit #EZER on Libera for any questions (9:00-23:00 UTC+1); you will need to be registered to join.
