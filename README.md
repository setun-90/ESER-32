# Zuse Elektra emulator
The Zuse Elektra is a system family from Zuse Gerätebau implementing the ESER/OSER architecture (a fictionalized version of the real-life [ESER](https://en.wikipedia.org/wiki/ESER); see [below](#background)). Released in 1964, it has diversified into a line of mainframes, superminis, workstations and servers. The English Electric Electra is a licensed clone.

This emulator emulates a 32-bit Zuse Elektra.
## Status
* Memory is verified; all specified features work, but some implementation features need to be frozen (such as cache sizing and replacement algorithms).
* Channel units are in progress; tests and devices still need to be written.
  * The Datei device needs a decent ESER test program.
* Processing units are in progress; tests need to be written and some aspects need to be frozen, such as cache sizes.
* The console needs to be expanded with commands for device control.

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

If loading proceeds without errors, you will reach the operator's console:
```
>> _
```
### Commands
* `g`: prints the highest physical address.
* `l $n $a`: prints *n* bytes located at address *a*.
* `s $a $w`: writes *w* to address *a*.
* `e`: lists active interrupt port physical addresses.
* `an $a`: powers on the unit whose interrupt port address is *a*.
* `ab $a`: powers off the unit whose interrupt port address is *a*.

### Responses
* `   `: No error.
* `:: `: Notice.
* `!! `: Error.
* `?? `: Bad command.


## Building
### Systems
CMake is used to simplify cross-platform building; nevertheless, some platform-specific adaptations are employed to simplify the process itself. There are three main supported build types: Release, RelWithDebInfo or Debug.

In particular, don't forget to add appropriate flags for your CPU in the `CXXFLAGS` environment variable, mainly for alignment. For example, one might use the following on the Intel i7-8565U:
```
$ CXXFLAGS="-falign-functions=64 -falign-jumps=64:40:16 -Wa,-mbranches-within-32B-boundaries"
```

while the following gives the best results on the i7-1195G7:
```
$ CXXFLAGS="-falign-functions=64 -falign-jumps=64"
```

#### CMake single-configuration backends (Make/NMake/MinGW, Ninja)
Create the directory `$type` corresponding to the build type.
```
$ mkdir $type
$ cd $type
$ CXXFLAGS="$specific_flags" cmake .. -DCMAKE_BUILD_TYPE=$type
$ cmake --build . -j$n
```

#### CMake multi-configuration backends (MSBuild, Xcode)
```
$ mkdir out
$ cd out
$ cmake ..
$ cmake --build . --config $type
```
##### Visual Studio debugging notes
Visual Studio debugging is experimental. In particular, passing the required arguments to programs properly under debugging is to still be figured out, as they must be launched from the IDE.

For Visual Studio with MSBuild:
1. Run the CMake generation (3rd) step;
2. Open `out\ESER-32.sln` in Visual Studio;
3. Locate the relevant project in the Solution Explorer and right-click on it;
4. Select Properties (bottom) in the drop-down menu;
5. Select Configuration Properties > Debugging in the left pane;
6. Fill in the Command Arguments field.

For Visual Studio with Ninja, a CMakePresets.json is being considered. In the mean time:
1. Open the local repository folder in Visual Studio;
2. Wait for Visual Studio to generate the Ninja build files;
3. Select the relevant target as startup item in the Select Startup Item dropdown;
4. Go to Main Menu > Debug > Debug and Launch Configuration for &lt;target&gt;;
5. Add the following line to each relevant target of `launch.vs.json` or `launch.json`:
   `"args": args`
   Where `args` is as follows:
   * for `durchgangeinheit.exe`: `["..\\lib\\prufung.dll"]`;
   * for `zuse.exe`: `["..\\..\\..\\..\\src\\beispiel.conf", "..\\lib\\"]`


### Dependencies
The emulator is written in C++11, but a C++20-compatible compiler is recommended for increased performance.

### Targets
* zuse: the emulator binary;
* gerate: the device binaries;
* prf/*: the individual tests.
  * speicher: tests memory, physical and virtual;
  * durchgangeinheit: tests channels and some essential devices (in progress);
  * recheneinheit: tests CPUs (in progress).

Install targets are being considered.

### Testing
The test order is:
1. `prf/speicher`;
2. `prf/durchgangeinheit ${Test_Device_Plugin}`;
3. `prf/recheneinheit`;
4. all device tests.

The test is successful if it returns normally, i.e. doesn't call terminate after throwing an exception or doesn't segfault. The tests can be run under gdb for debugging.


## Documentation
The principal documentation are the PDFs in doc/:
* 32.pdf: The ESER-32 specification, in English;
* de\_32.pdf / ae\_32.pdf: The original texts of the ESER-32 specification, in German and Ard English;
* 64.pdf: The ESER-64 specification, in English;
* de\_64.pdf / ae\_64.pdf: The original texts of the ESER-64 specification, in German and Ard English.

### Contributing
If you wish to provide a new device, just create a folder named `${Device_Name}` in `src/gerat/` with a CMakeLists.txt file containing at least the following:
```
cmake_minimum_required(VERSION 3.18)
project(${Device_Name} CXX)

add_library(${Device_Name} MODULE
	${Sources}
)
target_link_libraries(${Device_Name} PRIVATE
	kern
)
compile_options(${Device_Name})
add_dependencies(zuse ${Device_Name})
```


## Further information
Visit #ESER on Libera for any questions (9:00-23:00 UTC+1); you will need to be registered to join.

## Background
This emulator is the union of two projects:
1. An example of a paged architecture with [capability-based addressing](https://en.wikipedia.org/wiki/Capability-based_addressing), which I discovered in 2020 and wanted to do as the subject for my PhD, but never found the tutor for; and
2. A Prussian computer system in an alternate history of Earth centered around the formation of Russia in the Bronze Age, the survival of the Roman Empire, an alternate technological ecosystem and a lack of consumerism and liberalism.

I do not have any shorter summary.
