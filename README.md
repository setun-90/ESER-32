# Zuse Elektra emulator
The Zuse Elektra is a system family from Zuse Gerätebau implementing the EZER/OSER architecture. Released in 1964, it has diversified into a line of mainframes, superminis, workstations and servers. The English Electric Electra is a licensed clone.

This emulator emulates a 32-bit Zuse Elektra, configurable via a configuration file specifying:
* the amount of RAM in 4K pages;
* the units, in the format *k t [ p ]*, where
* * *k* is the type of unit (**r** for processing, **d** for channel);
* * *t* is the interrupt port address, in hex;
* * *p* are the parameters for each unit.

## Building

### Dependencies
The emulator is written in C++11, but a C++20-compatible compiler is recommended for increased performance.

### Targets

## Documentation
The principal documentation are the PDFs in doc/:
* 32.pdf: The EZER-32 specification, in English;
* de\_32.pdf / ae\_32.pdf: The original texts of the EZER-32 specification, in German and Ard English;
* 64.pdf: The EZER-64 specification, in English;
* de\_64.pdf / ae\_32.pdf: The original texts of the EZER-64 specification, in German and Ard English;

## Testing
Tests in prf/ are intended to be run in this order:
* speicher (tests memory);
* durchgangeinheit (tests channels and some essential devices) (in progress);
* recheneinheit (tests CPUs) (upcoming).
