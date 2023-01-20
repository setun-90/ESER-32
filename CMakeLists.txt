cmake_minimum_required(VERSION 3.18)
project(ESER-32 CXX)

set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY lib)
set(CMAKE_SHARED_MODULE_PREFIX )

function(compile_options Target)
	if(NOT TARGET "${Target}")
		return()
	endif()

	set(is_debug      "$<CONFIG:Debug>")
	set(is_release    "$<CONFIG:Release>")
	set(is_gnu        "$<CXX_COMPILER_ID:GNU>")
	set(is_msvc       "$<CXX_COMPILER_ID:MSVC>")
	set(is_clang      "$<CXX_COMPILER_ID:Clang>")
	set(is_appleclang "$<CXX_COMPILER_ID:AppleClang>")
	set(is_anyclang   "$<CXX_COMPILER_ID:Clang,AppleClang>")
	set(is_anynix     "$<CXX_COMPILER_ID:GNU,Clang,AppleClang>")

	set_target_properties("${Target}" PROPERTIES
		CXX_VISIBILITY_PRESET     hidden
		VISIBILITY_INLINES_HIDDEN ON
	)

	target_include_directories("${Target}" PRIVATE
		"$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>"
	)
	target_compile_options("${Target}" PRIVATE
		$<${is_anynix}:
			-Wall
			-Wextra
			-Wpedantic
		>
		$<${is_msvc}:
			/Wall
			/permissive-
		>


		$<${is_anynix}:
			-march=native
			-pipe
			-falign-functions=32
		>
		$<${is_gnu}:
			-falign-loops=32:24:16
			-falign-jumps=32:24:16
			-Wa,-mbranches-within-32B-boundaries
			-mno-direct-extern-access
		>
		$<${is_anyclang}:
			-falign-loops=32
			-fno-direct-access-external-data
		>


		$<$<AND:${is_release},${is_gnu}>:
			-fno-reorder-blocks-and-partition
			-fno-reorder-functions
			-ftree-cselim
			-flive-range-shrinkage
			-fpredictive-commoning
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
			-fasynchronous-unwind-tables
		>
		$<$<AND:${is_release},${is_anyclang}>:
			-flto=thin
			-fno-plt
		>
		$<$<AND:${is_release},${is_msvc}>:
			/MD
			/O2
			/Zi
		>


		$<$<AND:${is_debug},${is_anynix}>:
			-Og
			-ggdb
			-fsanitize=address
		>
		$<$<AND:${is_debug},${is_msvc}>:
			/MDd
			/Od
			/ZI
			/fsanitize=address
		>
	)

	target_link_options("${Target}" PRIVATE
		$<${is_anynix}:LINKER:-O1,--as-needed,--sort-common,-z,relro,-z,combreloc -ldl -pthread>
		$<$<AND:${is_release},${is_anynix}>:LINKER:-s>
		$<$<AND:${is_debug},${is_anynix}>:-fsanitize=address>
	)
endfunction()

# Targets
### Core
add_library(kern OBJECT EXCLUDE_FROM_ALL
	src/host.cc
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
