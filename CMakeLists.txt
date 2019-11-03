
cmake_minimum_required(VERSION 3.12 FATAL_ERROR)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF) # -std=c++11 instead of -std=gnu++11


function(CheckSimdOptions _enableOpt _assumeOpt)
  if (${_assumeOpt} AND (NOT ${_enableOpt}))
    message(WARNING "Forcing ${_enableOpt}=ON because ${_assumeOpt}=ON")
    set(${_enableOpt} ON PARENT_SCOPE)
  endif()
endfunction()

#---------------------------------------------------------------------
#
# Project
#

project(libfar)

# Version
set(LIBFAR_MAJOR_VERSION 0)
set(LIBFAR_MINOR_VERSION 1)
set(LIBFAR_PATCH_VERSION 0)
set(LIBFAR_EXTRA_VERSION "" CACHE STRING "Extra version string")
set(LIBFAR_VERSION "${LIBFAR_MAJOR_VERSION}.${LIBFAR_MINOR_VERSION}")
set(LIBFAR_VERSION "${LIBFAR_VERSION}.${LIBFAR_PATCH_VERSION}${LIBFAR_EXTRA_VERSION}")

# Options
option(ENABLE_SSE   "Enable runtime check for SSE support."    ON)
option(ENABLE_SSE2  "Enable runtime check for SSE2 support."   ON)
option(ENABLE_SSE3  "Enable runtime check for SSE3 support."   ON)
option(ENABLE_SSSE3 "Enable runtime check for SSSE3 support."  ON)
option(ENABLE_SSE41 "Enable runtime check for SSE4.1 support." ON)
option(ENABLE_SSE42 "Enable runtime check for SSE4.2 support." ON)
option(ENABLE_AVX   "Enable runtime check for AVX support."    ON)
option(ENABLE_AVX2  "Enable runtime check for AVX2 support."   ON)
option(ASSUME_SSE   "Assume compile-time SSE support."    ON)
option(ASSUME_SSE2  "Assume compile-time SSE2 support."   ON)
option(ASSUME_SSE3  "Assume compile-time SSE3 support."   OFF)
option(ASSUME_SSSE3 "Assume compile-time SSSE3 support."  OFF)
option(ASSUME_SSE41 "Assume compile-time SSE4.1 support." OFF)
option(ASSUME_SSE42 "Assume compile-time SSE4.2 support." OFF)
option(ASSUME_AVX   "Assume compile-time AVX support."    OFF)
option(ASSUME_AVX2  "Assume compile-time AVX2 support."   OFF)

# Platform
if ( APPLE )
    if ( "${CMAKE_SYSTEM_NAME}" MATCHES "Darwin" )
       set( LIBFAR_MACOSX TRUE )
       set( LIBFAR_PLATFORM "MacOSX" )
       set( LIBFAR_EXTRA_VERSION "-osx64" CACHE STRING "Extra version string" FORCE )
    endif ( "${CMAKE_SYSTEM_NAME}" MATCHES "Darwin")
elseif ( WIN32 OR CYGWIN OR MINGW OR MSYS )
    set( LIBFAR_WINDOWS TRUE )
    set( LIBFAR_PLATFORM "Windows" )
    set( LIBFAR_EXTRA_VERSION "-win64" CACHE STRING "Extra extra version string" FORCE )
elseif ( "${CMAKE_SYSTEM_NAME}" MATCHES "Linux" )
    set( LIBFAR_LINUX TRUE )
    set( LIBFAR_PLATFORM "Linux" )
    set( LIBFAR_EXTRA_VERSION "-linux64" CACHE STRING "Extra extra version string" FORCE )
endif ( APPLE )
message( STATUS "Platform: ${LIBFAR_PLATFORM}" )
message( STATUS "Extra version: ${LIBFAR_EXTRA_VERSION}" )

# Check coherence of SIMD options (ASSUME_xxx => ENABLE_xxx)
CheckSimdOptions(ENABLE_SSE   ASSUME_SSE  )
CheckSimdOptions(ENABLE_SSE2  ASSUME_SSE2 )
CheckSimdOptions(ENABLE_SSE3  ASSUME_SSE3 )
CheckSimdOptions(ENABLE_SSSE3 ASSUME_SSSE3)
CheckSimdOptions(ENABLE_SSE41 ASSUME_SSE41)
CheckSimdOptions(ENABLE_SSE42 ASSUME_SSE42)
CheckSimdOptions(ENABLE_AVX   ASSUME_AVX  )
CheckSimdOptions(ENABLE_AVX2  ASSUME_AVX2 )

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

add_library(libfar STATIC
    "src/adapt.c"
    "src/cpuid.c"
    "src/resample.c"
    "src/fpu/adapt_fpu.c"
    "src/fpu/resample_fpu.c"
    "src/sse41/adapt_sse41.c"
    "src/sse41/resample_sse41.c"
    "include/libfar.h"
    "include/libfar/libfar_config.h"
    "include/libfar/libfar_version.h"
    "include/libfar/adapt.h"
    "include/libfar/cpuid.h"
    "include/libfar/resample.h"
    "include/libfar/fpu/adapt_fpu.h"
    "include/libfar/fpu/resample_fpu.h"
    "include/libfar/sse41/adapt_sse41.h"
    "include/libfar/sse41/resample_sse41.h"
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

gtest_add_tests(TARGET libfar_tests)
