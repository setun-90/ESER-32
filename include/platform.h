#ifndef   ZUSE_PLAT_H
#define   ZUSE_PLAT_H



// Copyright 2020, 2022 Daniel Campos do Nascimento

// Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

// The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.



/* ESER Platform definitions */
/*** There are three different platform characteristics:
     OS APIs
     Plugin format
     Compiler features */

/*** Operating systems */
#if defined(__unix__) || defined(__APPLE__) && defined(__MACH__)
#	define ZUSE_POSIX
#elif defined(_WIN32) || defined(_WIN64)
#	define ZUSE_WINDOWS
#endif


/***** These currently also determine the plugin format
       (PE/COFF/ELF) insofar as each OS has a preferred
       format - I personally wish this weren't so */
#if defined(ZUSE_POSIX)
#	define ZUSE_PLUGIN_SUFFIX ".so"
#elif defined(ZUSE_WINDOWS)
#	define ZUSE_PLUGIN_SUFFIX ".dll"
#	define WIN32_LEAN_AND_MEAN
#	ifndef UNICODE
#		define UNICODE
#	endif
#endif

/*** Compiler features */
#if defined(__GNUC__)
#	define ABBAU extern "C" __attribute__((visibility("default")))
#elif defined(_MSC_VER)
#	define ABBAU __declspec(dllexport)
#else
#	define ABBAU
#endif



#endif /* ZUSE_PLAT_H */
