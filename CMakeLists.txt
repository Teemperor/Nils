cmake_minimum_required(VERSION 3.0)
project(nils)

set(CMAKE_CXX_STANDARD 14)

enable_testing()

include_directories(extern)

add_subdirectory(lib)
add_subdirectory(unittests)
add_subdirectory(test)
add_subdirectory(main)