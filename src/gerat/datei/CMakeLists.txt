cmake_minimum_required(VERSION 3.18)
project(Dateigerat CXX)

add_library(datei MODULE
	src.cc
)
target_link_libraries(datei PRIVATE
	kern
)
compile_options(datei)
add_dependencies(zuse datei)
