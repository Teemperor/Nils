
add_library(nilsLib
    Nils.cpp
    Nils.h
    Utils.cpp
    Utils.h
    Pass.cpp
    Pass.h
    PassManager.cpp
    PassManager.h

    NativePasses/DeleteLinePass.cpp
    NativePasses/DeleteLinePass.h
    NativePasses/DeleteCharRangePass.cpp
    NativePasses/DeleteCharRangePass.h
    PassResult.cpp
    PassResult.h
    ExecutablePass.cpp
    ExecutablePass.h)

target_link_libraries(nilsLib)