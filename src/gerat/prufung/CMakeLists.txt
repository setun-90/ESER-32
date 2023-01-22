cmake_minimum_required(VERSION 3.18)
project(Prufungsgerat CXX)

add_library(prufung MODULE
	src.cc
)
target_link_libraries(prufung PRIVATE
	kern
)
compile_options(prufung)
add_dependencies(zuse prufung)
