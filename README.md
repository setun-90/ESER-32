# Zuse Elektra emulator
The Zuse Elektra is a system family from Zuse Gerätebau implementing the EZER/OSER architecture. Released in 1964, it has diversified into a line of mainframes, superminis, workstations and servers. The English Electric Electra is a licensed clone.

This emulator emulates a 32-bit Zuse Elektra.
## Status
* Memory is verified; all specified features work, but some implementation features need to be frozen (such as cache sizing and replacement algorithms).
* Channel units are in progress; tests and devices still need to be written.
  * The actual directory structure for device sources needs to be replaced.
  * The POSIX device needs a decent EZER test program.
* Processing units are in progress; tests need to be written and some aspects need to be frozen, such as cache sizes.

## Running
Run it as:

`${PATH}/zuse ${Config_File_Path} ${Device_Plugin_Directory}`

The config file specifies the following:
* the amount of RAM in 4K pages;
* the units, in the format *k t [ p ]*, where
  * *k* is the type of unit (**r** for processing, **d** for channel);
  * *t* is the interrupt port address, in hex;
  * *p* are the parameters for each unit.

Channel units in turn take the basename of a device plugin (either a .so on POSIX or a .dll on Windows) as the first parameter of *p*.

## Building
### Systems
CMake is used to simplify cross-platform building; nevertheless, some platform-specific adaptations are employed to simplify the process itself.
#### CMake single-configuration backends (Make, NMake, MinGW)
Create the directory `$type` corresponding to the build type: either release or build.
`$ mkdir $type`
`$ cd $type`
`$ cmake ..`
`$ make -j$n`

#### CMake multi-configuration (MSBuild/Visual Studio and Xcode)
`$ mkdir build`
`$ cd build`
`$ cmake ..`
`$ cmake --build . --config $type`


### Dependencies
The emulator is written in C++11, but a C++20-compatible compiler is recommended for increased performance.

### Targets
* zuse: the emulator binary;
* gerate: the devices binaries
* prf/*: the individual tests.
  * speicher: tests memory, physical and virtual;
  * durchgangeinheit: tests channels and some essential devices (in progress);
  * recheneinheit: tests CPUs (in progress).

Install targets are being considered.

### Testing
The test order is:
1. prf/speicher;
2. prf/durchgangeinheit;
3. prf/recheneinheit;
4. all device tests.

The test is successful if it returns normally, i.e. doesn't call terminate after throwing an exception or doesn't segfault. The tests can be run under gdb for debugging.


## Documentation
The principal documentation are the PDFs in doc/:
* 32.pdf: The EZER-32 specification, in English;
* de\_32.pdf / ae\_32.pdf: The original texts of the EZER-32 specification, in German and Ard English;
* 64.pdf: The EZER-64 specification, in English;
* de\_64.pdf / ae\_32.pdf: The original texts of the EZER-64 specification, in German and Ard English.

## Further information
Visit #EZER on Libera for any questions (9:00-23:00 UTC+1); you will need to be registered to join.
