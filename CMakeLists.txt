cmake_minimum_required(VERSION 3.18)
project(EZER-32 CXX)

set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED 11)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY lib)
set(CMAKE_SHARED_MODULE_PREFIX )
if(CMAKE_BINARY_DIR MATCHES debug)
	set(CMAKE_BUILD_TYPE Debug)
elseif(CMAKE_BINARY_DIR MATCHES release)
	set(CMAKE_BUILD_TYPE Release)
endif()

function(common_compile_options Target)
	if(NOT TARGET "${Target}")
		return()
	endif()
	target_include_directories("${Target}" PRIVATE
		include
	)
	if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU"
	   OR CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
		target_compile_options("${Target}" PRIVATE
			-Wall -Wextra -Wpedantic
			-march=native
			-pipe
			-falign-functions=32
			-falign-loops=32
	  	)
		if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
			target_compile_options("${Target}" PRIVATE
				-falign-jumps=32
				-Wa,-mbranches-within-32B-boundaries
				-mno-direct-extern-access
			)
		elseif(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
			target_compile_options("${Target}" PRIVATE
				-fno-direct-access-external-data
			)
		endif()
		target_link_options("${Target}" PRIVATE
			LINKER:-O1,--as-needed,--sort-common,-z,relro,-z,combreloc
		)
		target_link_libraries("${Target}" PRIVATE -ldl -pthread)
	elseif(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
		target_compile_options("${Target}" PRIVATE
			/Wall
			/permissive-
		)
	elseif(CMAKE_CXX_COMPILER_ID STREQUAL "Intel")
		target_compile_options("${Target}" PRIVATE
		)
	endif()
endfunction()
function(compile_options Target)
	if(NOT TARGET "${Target}")
		return()
	endif()
	common_compile_options("${Target}")
	if(CMAKE_BUILD_TYPE STREQUAL Release)
		if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU"
		   OR CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
			if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
				target_compile_options("${Target}" PRIVATE
					-fvisibility=hidden
					-fno-reorder-blocks-and-partition
					-fno-reorder-functions
					-fira-region=mixed
					-ftree-cselim
					-flive-range-shrinkage
					-fpredictive-commoning
					-ftree-loop-distribution
					-fsched-pressure
					-fweb
					-frename-registers
					-fipa-pta
					-flto=auto
					-flto-partition=one
					-floop-nest-optimize
					-fgraphite-identity
					-fno-plt
					-fno-semantic-interposition
					-fdevirtualize-at-ltrans
				)
			elseif(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
				target_compile_options("${Target}" PRIVATE
					-flto=thin
					-fno-plt
				)
			endif()
			target_link_options("${Target}" PRIVATE
				LINKER:-s
			)
		elseif(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
			target_compile_options("${Target}" PRIVATE
				/MD
				/O2
				/Zi
			)
		endif()
	elseif(CMAKE_BUILD_TYPE STREQUAL Debug)
		if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU"
		   OR CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
			target_compile_options("${Target}" PRIVATE
				-Og
				-ggdb
				-fsanitize=address
			)
			target_link_options("${Target}" PRIVATE
				-fsanitize=address
			)
		elseif(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
			target_compile_options("${Target}" PRIVATE
				/MDd
				/Od
				/ZI
				/fsanitize=address
			)
		endif()
	endif()
endfunction()

# Targets
### Core
add_library(kern OBJECT EXCLUDE_FROM_ALL
	src/verbindung.cc
	src/wahrspeicher.cc
	src/sonderfalle.cc
	src/verwandlungseinheit.cc
	src/einheit.cc
	src/recheneinheit.cc
	src/durchgangeinheit.cc
)
compile_options(kern)

### Emulator binary
add_executable(zuse
	src/zuse.cc
)
target_link_libraries(zuse PRIVATE
	kern
)
compile_options(zuse)
set_target_properties(zuse PROPERTIES RUNTIME_OUTPUT_DIRECTORY bin)

### Device binaries
file(GLOB GERATE src/gerat/*.cmake)
foreach(Cmake ${GERATE})
	include("${Cmake}")
endforeach()

### Tests
add_executable(durchgangeinheit
	prf/durchgangeinheit.cc
)
target_link_libraries(durchgangeinheit PRIVATE
	kern
)
add_dependencies(durchgangeinheit
	prufung
)
compile_options(durchgangeinheit)

add_executable(recheneinheit
	prf/recheneinheit.cc
)
target_link_libraries(recheneinheit PRIVATE
	kern
)
compile_options(recheneinheit)

add_executable(speicher
	prf/speicher.cc
)
target_link_libraries(speicher PRIVATE
	kern
)
compile_options(speicher)
set_target_properties(speicher durchgangeinheit recheneinheit PROPERTIES RUNTIME_OUTPUT_DIRECTORY prf)
