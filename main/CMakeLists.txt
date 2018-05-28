
include_directories(../lib)

find_package( Threads )

add_executable(nils main.cpp)
target_link_libraries(nils nilsLib ${CMAKE_THREAD_LIBS_INIT})