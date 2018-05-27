cmake_minimum_required(VERSION 3.0)
project(nils)

set(CMAKE_CXX_STANDARD 14)

include_directories(lib)
add_subdirectory(lib)

enable_testing()

add_subdirectory(unittests)

add_subdirectory(test)

add_executable(nils main.cpp)
target_link_libraries(nils nilsLib)