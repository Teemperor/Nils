
include_directories(.)

add_library(nilsLib
    Nils.cpp
    Nils.h
    NilsFeedback.cpp
    NilsFeedback.h
    Utils.cpp
    Utils.h
    Pass.cpp
    Pass.h
    PassManager.cpp
    PassManager.h
    Job.cpp
    Job.h
    Process.cpp
    Process.h
    PassResult.cpp
    PassResult.h
    ExecutablePass.cpp
    ExecutablePass.h

    NativePasses/DeleteLinePass.cpp
    NativePasses/DeleteLinePass.h
    NativePasses/DeleteCharRangePass.cpp
    NativePasses/DeleteCharRangePass.h
    NativePasses/PassUtils.cpp
    NativePasses/PassUtils.h
    NativePasses/RemoveTokenPass.cpp
    NativePasses/RemoveTokenPass.h
    NativePasses/RenameTokenPass.cpp
    NativePasses/RenameTokenPass.h)

target_link_libraries(nilsLib)