cmake_minimum_required(VERSION 3.18)
project(ESER-32 CXX)

set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY lib)
set(CMAKE_SHARED_MODULE_PREFIX )

function(common_compile_options Target)
	if(NOT TARGET "${Target}")
		return()
	endif()
	target_include_directories("${Target}" PRIVATE
		"$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>"
	)
	set_target_properties("${Target}" PROPERTIES
		CXX_VISIBILITY_PRESET     hidden
		VISIBILITY_INLINES_HIDDEN ON
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
	if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU"
	   OR CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
		if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
			target_compile_options("${Target}" PRIVATE
				$<$<CONFIG:Release>:
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
				-fdevirtualize-at-ltrans>

				$<$<CONFIG:Debug>:
				-Og
				-ggdb
				-fsanitize=address>
			)
		elseif(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
			target_compile_options("${Target}" PRIVATE
				$<$<CONFIG:Release>:
				-flto=thin
				-fno-plt>

				$<$<CONFIG:Debug>:
				-Og
				-ggdb
				-fsanitize=address>
			)
		endif()
		target_link_options("${Target}" PRIVATE
			$<$<CONFIG:Release>:LINKER:-s>
			$<$<CONFIG:Debug>:-fsanitize=address>
		)
	elseif(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
		target_compile_options("${Target}" PRIVATE
			$<$<CONFIG:Release>:
			/MD
			/O2
			/Zi>

			$<$<CONFIG:Debug>:
			/MDd
			/Od
			/ZI
			/fsanitize=address>
		)
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
