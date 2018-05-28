
include_directories(../lib)

add_executable(nils main.cpp)
target_link_libraries(nils nilsLib)