macro(subdirlist result curdir)
  file(GLOB children RELATIVE "${curdir}" "${curdir}/*")
  set(dirlist "")
  foreach(child ${children})
    if(NOT "${child}" MATCHES "\\.expected$")
      if(IS_DIRECTORY "${curdir}/${child}")
        list(APPEND dirlist "${child}")
      endif()
    endif()
  endforeach()
  set(${result} "${dirlist}")
endmacro()

subdirlist(SUBDIRS "${CMAKE_CURRENT_SOURCE_DIR}")

foreach(subdir ${SUBDIRS})
  get_filename_component(source_path "${subdir}"
      REALPATH BASE_DIR "${CMAKE_CURRENT_SOURCE_DIR}")

  get_filename_component(target_path "${subdir}"
      REALPATH BASE_DIR "${CMAKE_BINARY_DIR}")

  add_test(NAME "Test${subdir}"
           COMMAND
           "${CMAKE_CURRENT_SOURCE_DIR}/run.sh"
           "${CMAKE_BINARY_DIR}/nils"
           "${source_path}"
           "${target_path}"
      )
endforeach()