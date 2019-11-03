
cmake_minimum_required(VERSION 3.12 FATAL_ERROR)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF) # -std=c++11 instead of -std=gnu++11

function(SimdOption _varName _defaultValue _featureName)
  set(FAR_SIMD_${_varName} ${_defaultValue} CACHE STRING "Configure support for ${_featureName}. Valid values are 'Disabled' to compile out support, 'RuntimeCheckEnabled' to use CPUID to dymically check support at runtime, and 'CompileTimeAssumed' to assume support at compile time.")
  set_property(CACHE FAR_SIMD_${_varName} PROPERTY STRINGS Disabled RuntimeCheckEnabled CompileTimeAssumed)
  set(VAL ${FAR_SIMD_${_varName}})
  if ("${VAL}" STREQUAL "RuntimeCheckEnabled")
    set(FAR_ENABLE_${_varName} 1 PARENT_SCOPE)
  elseif ("${VAL}" STREQUAL "CompileTimeAssumed")
    set(FAR_ENABLE_${_varName} 1 PARENT_SCOPE)
    set(FAR_ASSUME_${_varName} 1 PARENT_SCOPE)
  endif()
endfunction()

function(LanguageOption)
  set(FAR_LANG "C" CACHE STRING "Library language")
  set_property(CACHE FAR_LANG PROPERTY STRINGS "C" "C++")
  if ("${FAR_LANG}" STREQUAL "C++")
    set(FAR_LANG_CPP 1 PARENT_SCOPE)
  else()
    set(FAR_LANG_C 1 PARENT_SCOPE)
  endif()
endfunction()

#---------------------------------------------------------------------
#
# Project
#

project(libfar)

# Version
set(FAR_MAJOR_VERSION 0)
set(FAR_MINOR_VERSION 1)
set(FAR_PATCH_VERSION 0)
set(FAR_EXTRA_VERSION "" CACHE STRING "Extra version string")
set(FAR_VERSION "${FAR_MAJOR_VERSION}.${FAR_MINOR_VERSION}")
set(FAR_VERSION "${FAR_VERSION}.${FAR_PATCH_VERSION}${FAR_EXTRA_VERSION}")

# SIMD options
SimdOption(SSE CompileTimeAssumed "SSE")
SimdOption(SSE2 CompileTimeAssumed "SSE2")
SimdOption(SSE3 RuntimeCheckEnabled "SSE3")
SimdOption(SSSE3 RuntimeCheckEnabled "SSSE3")
SimdOption(SSE41 RuntimeCheckEnabled "SSE4.1")
SimdOption(SSE42 RuntimeCheckEnabled "SSE4.2")
SimdOption(AVX RuntimeCheckEnabled "AVX")
SimdOption(AVX2 RuntimeCheckEnabled "AVX2")
LanguageOption()

# Library language
if (FAR_LANG_C)
  message(STATUS "Compiling libfar as a C library.")
  if (MSVC)
    set(FAR_CXXFLAGS "${FAR_CXXFLAGS} /TC")
  else()
    set(FAR_CXXFLAGS "${FAR_CXXFLAGS} -x c")
  endif()
elseif (FAR_LANG_CPP)
  message(STATUS "Compiling libfar as a C++ library.")
  if (MSVC)
    set(FAR_CXXFLAGS "${FAR_CXXFLAGS} /TP")
  else()
    set(FAR_CXXFLAGS "${FAR_CXXFLAGS} -x c++")
  endif()
endif()

# Platform
if ( APPLE )
    if ( "${CMAKE_SYSTEM_NAME}" MATCHES "Darwin" )
       set( FAR_MACOSX TRUE )
       set( FAR_PLATFORM "MacOSX" )
       set( FAR_EXTRA_VERSION "-osx64" CACHE STRING "Extra version string" FORCE )
    endif ( "${CMAKE_SYSTEM_NAME}" MATCHES "Darwin")
elseif ( WIN32 OR CYGWIN OR MINGW OR MSYS )
    set( FAR_WINDOWS TRUE )
    set( FAR_PLATFORM "Windows" )
    set( FAR_EXTRA_VERSION "-win64" CACHE STRING "Extra extra version string" FORCE )
elseif ( "${CMAKE_SYSTEM_NAME}" MATCHES "Linux" )
    set( FAR_LINUX TRUE )
    set( FAR_PLATFORM "Linux" )
    set( FAR_EXTRA_VERSION "-linux64" CACHE STRING "Extra extra version string" FORCE )
endif ( APPLE )
message( STATUS "Platform: ${FAR_PLATFORM}" )
message( STATUS "Extra version: ${FAR_EXTRA_VERSION}" )

# Use solution directories in Visual Studio
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# Remove ZERO_CHECK project from Visual Studio solution
set(CMAKE_SUPPRESS_REGENERATION TRUE)

# Replace the Debug and Release of Cmake by our own configs.
include(tools/build/buildconfig.cmake)

message(STATUS "Compiler: ${CMAKE_CXX_COMPILER_ID}")
if (CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /W4 /WX /GF /permissive-")
    set(CMAKE_CXX_FLAGS "${CMAKE_C_FLAGS} /W4 /WX /GF /permissive-")
    set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} /Oi /Oy /Gy")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /Oi /Oy /Gy")
    set(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE}")
elseif (CMAKE_CXX_COMPILER_ID MATCHES "Clang") # Both Clang and Apple-Clang
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Werror -msse4.1")
    set(CMAKE_CXX_FLAGS "${CMAKE_C_FLAGS} -Wall -Werror -msse4.1")
    set(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -flto=thin")
endif()

set(libfar_source_files
    src/adapt.c
    src/far_cpuid.c
    src/resample.c
    src/fpu/adapt_fpu.c
    src/fpu/resample_fpu.c
    src/sse41/adapt_sse41.c
    src/sse41/resample_sse41.c
)

add_library(libfar STATIC
    ${libfar_source_files}
    "include/libfar.h"
    "include/libfar/libfar_config.h"
    "include/libfar/libfar_version.h"
    "include/libfar/adapt.h"
    "include/libfar/far_cpuid.h"
    "include/libfar/resample.h"
    "include/libfar/fpu/adapt_fpu.h"
    "include/libfar/fpu/resample_fpu.h"
    "include/libfar/sse41/adapt_sse41.h"
    "include/libfar/sse41/resample_sse41.h"
)

# Source files have the .c extension, which defaults to C code.
# When compiling the library as C++, force the language.
if (FAR_LANG_CPP)
    set_source_files_properties(
        ${libfar_source_files}
        PROPERTIES LANGUAGE CXX
    )
endif()

# Assign SSE options
if (MSVC)
    # Anything to do?
else()
    set(FAR_SSE41_FLAG "-msse41")
endif()
set_source_files_properties(
    src/sse41/adapt_sse41.c
    src/sse41/resample_sse41.c
    PROPERTIES COMPILE_FLAGS "${FAR_SSE41_FLAG}"
)

target_include_directories(libfar
    PUBLIC
    include
    PRIVATE
    ${CMAKE_BINARY_DIR}
)

# Configure
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/src/libfar_config.h.in  ${CMAKE_CURRENT_SOURCE_DIR}/include/libfar/libfar_config.h )
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/src/libfar_version.h.in ${CMAKE_CURRENT_SOURCE_DIR}/include/libfar/libfar_version.h)

#---------------------------------------------------------------------
#
# Testing
#

include(GoogleTest)

add_executable(libfar_tests
    "test/test_adapt.cpp"
    "test/test_memory.cpp"
    "test/test_resample.cpp"
    "test/pch.h"
)

target_include_directories(libfar_tests
    PUBLIC
    include
)

gtest_add_tests(TARGET libfar_tests)
