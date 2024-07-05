cmake_minimum_required(VERSION 3.14)
project(libcppnet)

set(CMAKE_CXX_STANDARD 14)
set(PROJECT_NAME libcppnet)
add_compile_options(-Wall -Wextra -Wpedantic -pthread -fPIC -lrt)
add_compile_options("$<$<CONFIG:Release>:-03>")

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib)

enable_language(CXX)
set_property(GLOBAL PROPERTY USE_FOLDERS TRUE)

function(get_all_targets _result _dir)
    get_property(_subdirs DIRECTORY "${_dir}" PROPERTY SUBDIRECTORIES)
    foreach(_subdir IN LISTS _subdirs)
        get_all_targets(${_result}, "${_subdir}")
    endforeach()
    get_property(_sub_targets DIRECTORY "${_dir}" PROPERTY BUILDSYSTEM_TARGETS)
    set(${_result} ${${_result}} ${_sub_targets} PARENT_SCOPE)
endfunction()

add_subdirectory(cpp)