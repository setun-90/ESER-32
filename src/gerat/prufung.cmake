add_library(prufung MODULE
	src/gerat/prufung.cc
)
target_link_libraries(prufung PRIVATE
	kern
)
compile_options(prufung)
add_dependencies(zuse prufung)
