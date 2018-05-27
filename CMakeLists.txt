cmake_minimum_required(VERSION 3.11)
project(minimi)

set(CMAKE_CXX_STANDARD 14)

include_directories(lib)
add_subdirectory(lib)

add_subdirectory(unittests)

add_executable(nils main.cpp)
target_link_libraries(nils nilsLib)