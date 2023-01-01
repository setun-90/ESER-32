add_library(posix MODULE
	src/gerat/posix.cc
)
target_link_libraries(posix PRIVATE
	kern
)
compile_options(posix)
add_dependencies(zuse posix)
