project(path_comp)
add_library(path_comp STATIC
    src/interface.hpp
    src/impl.hpp
    src/gen_svg.hpp
    src/gen_svg.cpp
)

set_target_properties(path_comp PROPERTIES
    CXX_STANDARD 20
	CXX_STANDARD_REQUIRED ON
	CXX_EXTENSIONS OFF
	COMPILE_WARNING_AS_ERROR ON
)

target_include_directories(path_comp INTERFACE src)
