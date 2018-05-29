
function(add_unit_test Name)
  add_executable("${Name}" "${Name}.cpp")
  target_link_libraries("${Name}" nilsLib)

  add_test(NAME "${Name}" COMMAND "${Name}")
endfunction()

include_directories(../lib)

include_directories("${CMAKE_BINARY_DIR}/catch/")

if(NOT EXISTS "${CMAKE_BINARY_DIR}/catch/catch.hpp")
  file(MAKE_DIRECTORY "${CMAKE_BINARY_DIR}/catch/")

  file(DOWNLOAD "https://github.com/catchorg/Catch2/releases/download/v2.2.2/catch.hpp"
      "${CMAKE_BINARY_DIR}/catch/catch.hpp")
endif()
add_unit_test(ShellCommandTest)
add_unit_test(UtilsTest)
add_unit_test(PassUtilsTest)