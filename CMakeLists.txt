cmake_minimum_required (VERSION 2.6)
project (REBIND)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")

add_definitions(-std=c++11)
add_subdirectory (include)
add_subdirectory (examples/readme_examples)
add_subdirectory (examples/reverse_string)
add_subdirectory (test)
