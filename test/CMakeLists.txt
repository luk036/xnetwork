############################################################################
# Copyright (c) Johan Mabille, Sylvain Corlay and Wolf Vollprecht          #
# Copyright (c) QuantStack                                                 #
#                                                                          #
# Distributed under the terms of the BSD 3-Clause License.                 #
#                                                                          #
# The full license is in the file LICENSE, distributed with this software. #
############################################################################

cmake_minimum_required(VERSION 3.1)

find_package(doctest            REQUIRED)
find_package(Threads)

if (CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR)
    project(xnetwork-test)

    enable_testing()

    find_package(xnetwork REQUIRED CONFIG)
    find_package(nlohmann_json QUIET CONFIG)
    set(XNETWORK_INCLUDE_DIR ${xnetwork_INCLUDE_DIRS})
endif ()

if(NOT CMAKE_BUILD_TYPE)
    message(STATUS "Setting tests build type to Release")
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Choose the type of build." FORCE)
else()
    message(STATUS "Tests build type is ${CMAKE_BUILD_TYPE}")
endif()

include(CheckCXXCompilerFlag)

string(TOUPPER "${CMAKE_BUILD_TYPE}" U_CMAKE_BUILD_TYPE)

if(nlohmann_json_FOUND)
  add_definitions(-DHAVE_NLOHMANN_JSON)
endif()

if(CMAKE_CXX_COMPILER_ID MATCHES GNU OR CMAKE_CXX_COMPILER_ID MATCHES Intel)
    add_compile_options(-Wunused-parameter -Wextra -Wreorder -Wconversion -Wsign-conversion)

    if(NOT CMAKE_CXX_FLAGS MATCHES "-march")
        CHECK_CXX_COMPILER_FLAG(-march=native HAS_MARCH_NATIVE)
        if (HAS_MARCH_NATIVE)
            add_compile_options(-march=native)
        endif()
    endif()
    if (XNETWORK_DISABLE_EXCEPTIONS)
        add_compile_options(-fno-exceptions)
    endif()
endif()

if(CMAKE_CXX_COMPILER_ID MATCHES MSVC)
    add_compile_options(/EHsc /MP /bigobj)
    set(CMAKE_EXE_LINKER_FLAGS /MANIFEST:NO)
    if (XNETWORK_DISABLE_EXCEPTIONS)
        add_compile_options(/EHs-c-)
    endif()
endif()

if(CMAKE_CXX_COMPILER_ID MATCHES Clang)
  if(NOT WIN32)
    add_compile_options(-Wunused-parameter -Wextra -Wreorder -Wconversion -Wsign-conversion)

    if(NOT CMAKE_CXX_FLAGS MATCHES "-march")
        CHECK_CXX_COMPILER_FLAG(-march=native HAS_MARCH_NATIVE)
        if (HAS_MARCH_NATIVE)
            add_compile_options(-march=native)
        endif()
    endif()
    if (XNETWORK_DISABLE_EXCEPTIONS)
        add_compile_options(-fno-exceptions)
    endif()
  else() # we are using clang-cl
    add_compile_options(/EHsc /bigobj -fms-compatibility)
    set(CMAKE_EXE_LINKER_FLAGS /MANIFEST:NO)
    if (XNETWORK_DISABLE_EXCEPTIONS)
        add_compile_options(/EHs-c-)
    endif()
  endif()
endif()

find_package(Threads)

set(XNETWORK_TESTS
    test_graphh.cpp
    test_xn.cpp
)

if(nlohmann_json_FOUND)
    # Version up to 3.1.2 export the target `nlohmann_json`
    if(TARGET nlohmann_json)
      set(nlohmann_json_TARGET nlohmann_json)
    # Newer versions export the namespaced target `nlohmann_json::nlohmann_json`
    elseif(TARGET nlohmann_json::nlohmann_json)
      set(nlohmann_json_TARGET nlohmann_json::nlohmann_json)
    endif()
endif()

foreach(filename IN LISTS XNETWORK_TESTS)
    get_filename_component(targetname ${filename} NAME_WE)

    add_executable(${targetname} main.cpp  ${filename} ${XNETWORK_HEADERS})
    target_include_directories(${targetname} PRIVATE ${XNETWORK_INCLUDE_DIR})
    target_link_libraries(${targetname} xnetwork doctest::doctest Threads::Threads ${nlohmann_json_TARGET})

    add_test(NAME ${targetname} COMMAND ${targetname})
endforeach()

add_executable(test_xnetwork main.cpp ${XNETWORK_TESTS} ${XNETWORK_HEADERS})
target_include_directories(test_xnetwork PRIVATE ${XNETWORK_INCLUDE_DIR})
target_link_libraries(test_xnetwork xnetwork doctest::doctest Threads::Threads ${nlohmann_json_TARGET})

add_custom_target(xtest COMMAND test_xnetwork DEPENDS test_xnetwork)
add_test(NAME xtest COMMAND test_xnetwork)
