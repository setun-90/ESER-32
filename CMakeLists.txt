cmake_minimum_required(VERSION 3.18)
project(ESER-32 CXX)

set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "$<BUILD_INTERFACE:${CMAKE_BINARY_DIR}/lib>")
set(CMAKE_SHARED_MODULE_PREFIX )

set(is_debug          "$<CONFIG:Debug>")
set(is_release        "$<CONFIG:Release>")
set(is_relwithdebinfo "$<CONFIG:RelWithDebInfo>")
set(is_optimized      $<OR:${is_release},${is_relwithdebinfo}>)
set(is_gnu        "$<CXX_COMPILER_ID:GNU>")
set(is_msvc       "$<CXX_COMPILER_ID:MSVC>")
set(is_clang      "$<CXX_COMPILER_ID:Clang>")
set(is_appleclang "$<CXX_COMPILER_ID:AppleClang>")
set(is_anyclang   "$<CXX_COMPILER_ID:Clang,AppleClang>")
set(is_anynix     "$<CXX_COMPILER_ID:GNU,Clang,AppleClang>")

function(compile_options Target)
	if(NOT TARGET "${Target}")
		return()
	endif()

	set_target_properties("${Target}" PROPERTIES
		CXX_VISIBILITY_PRESET     hidden
		VISIBILITY_INLINES_HIDDEN ON
	)

	target_include_directories("${Target}" PRIVATE
		"$<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>"
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
		>

		$<$<AND:${is_optimized},${is_gnu}>:
			-flto=auto
			-flto-partition=one
			-fno-plt
			-fno-semantic-interposition
			-fdevirtualize-at-ltrans
			-fasynchronous-unwind-tables
		>
		$<$<AND:${is_optimized},${is_anyclang}>:
			-fno-direct-access-external-data
			-flto=thin
			-fno-plt
		>
		$<$<AND:${is_optimized},${is_msvc}>:
			/GL
			/Gw
		>

		$<$<AND:${is_debug},${is_anynix}>:
			-O0
			-ggdb
			-fsanitize=address
		>
		$<$<AND:${is_debug},${is_msvc}>:
			/Zi
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
file(GLOB GERATE src/gerat/*/)
foreach(GERAT ${GERATE})
	if(IS_DIRECTORY ${GERAT})
		add_subdirectory("${GERAT}")
	endif()
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
