#---------------------------------------------------------------------
#
# Build configurations
# - Debug
# - Profile
# - Release
#
#---------------------------------------------------------------------

#
# Note on preprocessor defines per build configuration
#
# The following prepocessor defines are defined:
# - Debug  : LIBFAR_DEBUG   + LIBFAR_HAS_PROFILING
# - Profile: LIBFAR_PROFILE + LIBFAR_HAS_PROFILING
# - Release: LIBFAR_RELEASE
#
# LIBFAR_HAS_PROFILING means that profiling is available (in Debug and Profile build configs).
# LIBFAR_PROFILE means that the build config is exactly the Profile config.
#

# Remove built-in configurations: Debug MinSizeRel Release RelWithDebInfo
unset(CMAKE_C_FLAGS_DEBUG CACHE)
unset(CMAKE_C_FLAGS_MINSIZEREL CACHE)
unset(CMAKE_C_FLAGS_RELEASE CACHE)
unset(CMAKE_C_FLAGS_RELWITHDEBINFO CACHE)
unset(CMAKE_CXX_FLAGS_DEBUG CACHE)
unset(CMAKE_CXX_FLAGS_MINSIZEREL CACHE)
unset(CMAKE_CXX_FLAGS_RELEASE CACHE)
unset(CMAKE_CXX_FLAGS_RELWITHDEBINFO CACHE)
unset(CMAKE_EXE_LINKER_FLAGS_DEBUG CACHE)
unset(CMAKE_EXE_LINKER_FLAGS_MINSIZEREL CACHE)
unset(CMAKE_EXE_LINKER_FLAGS_RELEASE CACHE)
unset(CMAKE_EXE_LINKER_FLAGS_RELWITHDEBINFO CACHE)
unset(CMAKE_MODULE_LINKER_FLAGS_DEBUG CACHE)
unset(CMAKE_MODULE_LINKER_FLAGS_MINSIZEREL CACHE)
unset(CMAKE_MODULE_LINKER_FLAGS_RELEASE CACHE)
unset(CMAKE_MODULE_LINKER_FLAGS_RELWITHDEBINFO CACHE)
unset(CMAKE_SHARED_LINKER_FLAGS_DEBUG CACHE)
unset(CMAKE_SHARED_LINKER_FLAGS_MINSIZEREL CACHE)
unset(CMAKE_SHARED_LINKER_FLAGS_RELEASE CACHE)
unset(CMAKE_SHARED_LINKER_FLAGS_RELWITHDEBINFO CACHE)
unset(CMAKE_STATIC_LINKER_FLAGS_DEBUG CACHE)
unset(CMAKE_STATIC_LINKER_FLAGS_MINSIZEREL CACHE)
unset(CMAKE_STATIC_LINKER_FLAGS_RELEASE CACHE)
unset(CMAKE_STATIC_LINKER_FLAGS_RELWITHDEBINFO CACHE)

# Set build configurations
set(CMAKE_CONFIGURATION_TYPES Debug Profile Release)
set(CMAKE_CONFIGURATION_TYPES "${CMAKE_CONFIGURATION_TYPES}" CACHE STRING "Build configurations" FORCE)
set(CMAKE_C_FLAGS "" CACHE STRING "" FORCE)
set(CMAKE_CXX_FLAGS "" CACHE STRING "" FORCE)
set(CMAKE_EXE_LINKER_FLAGS "" CACHE STRING "" FORCE)

if (NOT WIN32)
    set(FAR_CXXFLAGS "${FAR_CXXFLAGS} -std=c++11")
endif (NOT WIN32)

if(ENABLE_EXCEPTIONS)
    if (MSVC)
        set(FAR_CXXFLAGS "${FAR_CXXFLAGS} /EHsc")
    else (MSVC)
        set(FAR_CXXFLAGS "${FAR_CXXFLAGS} -fexceptions")
    endif (MSVC)
else()
    if (MSVC)
        set(FAR_CXXFLAGS "${FAR_CXXFLAGS}")
    else (MSVC)
        set(FAR_CXXFLAGS "${FAR_CXXFLAGS} -fno-exceptions")
    endif (MSVC)
endif()

# Set compiler flags
if ( MSVC )
    
    message(STATUS "Setting default Visual Studio build flags...")

    set(libfar_common_flags "/W4 /WX")
    set(libfar_debug_flags "/Od /Ob1 /MDd /Zi")
    set(libfar_release_flags "/Ox /Ob2 /Oi /Oy /MD")

    # Set dynamic linking to multithreaded CRT library (/MDd or /MD)
    set(CMAKE_C_FLAGS_DEBUG "${libfar_common_flags} ${libfar_debug_flags} -DLIBFAR_DEBUG -DLIBFAR_HAS_PROFILING" CACHE STRING "" FORCE)
    set(CMAKE_C_FLAGS_PROFILE "${libfar_common_flags} ${libfar_release_flags} -DNDEBUG -DLIBFAR_PROFILE -DLIBFAR_HAS_PROFILING" CACHE STRING "" FORCE)
    set(CMAKE_C_FLAGS_RELEASE "${libfar_common_flags} ${libfar_release_flags} -DNDEBUG -DLIBFAR_RELEASE" CACHE STRING "" FORCE)
    set(CMAKE_CXX_FLAGS_DEBUG "${libfar_common_flags} ${libfar_debug_flags} -DLIBFAR_DEBUG -DLIBFAR_HAS_PROFILING ${FAR_CXXFLAGS}" CACHE STRING "" FORCE)
    set(CMAKE_CXX_FLAGS_PROFILE "${libfar_common_flags} ${libfar_release_flags} -DNDEBUG -DLIBFAR_PROFILE -DLIBFAR_HAS_PROFILING ${FAR_CXXFLAGS}" CACHE STRING "" FORCE)
    set(CMAKE_CXX_FLAGS_RELEASE "${libfar_common_flags} ${libfar_release_flags} -DNDEBUG -DLIBFAR_RELEASE ${FAR_CXXFLAGS}" CACHE STRING "" FORCE)
    set(CMAKE_EXE_LINKER_FLAGS_DEBUG "/debug" CACHE STRING "" FORCE)
    set(CMAKE_EXE_LINKER_FLAGS_PROFILE "/Ox" CACHE STRING "" FORCE)
    set(CMAKE_EXE_LINKER_FLAGS_RELEASE "/Ox" CACHE STRING "" FORCE)
    set(CMAKE_MODULE_LINKER_FLAGS_DEBUG "/debug" CACHE STRING "" FORCE)
    set(CMAKE_MODULE_LINKER_FLAGS_PROFILE "/Ox" CACHE STRING "" FORCE)
    set(CMAKE_MODULE_LINKER_FLAGS_RELEASE "/Ox" CACHE STRING "" FORCE)
    set(CMAKE_SHARED_LINKER_FLAGS_DEBUG "/debug" CACHE STRING "" FORCE)
    set(CMAKE_SHARED_LINKER_FLAGS_PROFILE "/Ox" CACHE STRING "" FORCE)
    set(CMAKE_SHARED_LINKER_FLAGS_RELEASE "/Ox" CACHE STRING "" FORCE)

    message(STATUS "Debug  : ${CMAKE_CXX_FLAGS_DEBUG}")
    message(STATUS "Profile: ${CMAKE_CXX_FLAGS_PROFILE}")
    message(STATUS "Release: ${CMAKE_CXX_FLAGS_RELEASE}") 

else ( MSVC )
    message(STATUS "Setting default gcc/clang build flags...")
    set(CMAKE_C_FLAGS_DEBUG "-g -Og -DLIBFAR_DEBUG -DLIBFAR_HAS_PROFILING" CACHE STRING "" FORCE)
    set(CMAKE_C_FLAGS_PROFILE "-O3 -DNDEBUG -DLIBFAR_PROFILE -DLIBFAR_HAS_PROFILING" CACHE STRING "" FORCE)
    set(CMAKE_C_FLAGS_RELEASE "-O3 -DNDEBUG -DLIBFAR_RELEASE" CACHE STRING "" FORCE)
    set(CMAKE_CXX_FLAGS_DEBUG "-g -Og -DLIBFAR_DEBUG -DLIBFAR_HAS_PROFILING ${FAR_CXXFLAGS}" CACHE STRING "" FORCE)
    set(CMAKE_CXX_FLAGS_PROFILE "-O3 -DNDEBUG -DLIBFAR_PROFILE -DLIBFAR_HAS_PROFILING ${FAR_CXXFLAGS}" CACHE STRING "" FORCE)
    set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG -DLIBFAR_RELEASE ${FAR_CXXFLAGS}" CACHE STRING "" FORCE)
endif ( MSVC )

# # Add "bundle_loader" flag pointing to main executable to prevent undefined
# # symbol link error when importing symbols from main executable into bundle
# if ( APPLE )
#   set(CMAKE_MODULE_LINKER_FLAGS_DEBUG "-bundle_loader ../bin/libfar_debug")
#   set(CMAKE_MODULE_LINKER_FLAGS_PROFILE "-bundle_loader ../bin/libfar_profile")
#   set(CMAKE_MODULE_LINKER_FLAGS_RELEASE "-bundle_loader ../bin/libfar")
# endif ( APPLE )

# Hide advanced flags in CMake GUIs
mark_as_advanced(
    CMAKE_C_FLAGS_DEBUG
    CMAKE_C_FLAGS_PROFILE
    CMAKE_C_FLAGS_RELEASE
    CMAKE_CXX_FLAGS_DEBUG
    CMAKE_CXX_FLAGS_PROFILE
    CMAKE_CXX_FLAGS_RELEASE
    CMAKE_EXE_LINKER_FLAGS_DEBUG
    CMAKE_EXE_LINKER_FLAGS_PROFILE
    CMAKE_EXE_LINKER_FLAGS_RELEASE
    CMAKE_MODULE_LINKER_FLAGS_DEBUG
    CMAKE_MODULE_LINKER_FLAGS_PROFILE
    CMAKE_MODULE_LINKER_FLAGS_RELEASE
    CMAKE_SHARED_LINKER_FLAGS_DEBUG
    CMAKE_SHARED_LINKER_FLAGS_PROFILE
    CMAKE_SHARED_LINKER_FLAGS_RELEASE
)

# Set default build type; usually overridden by user in IDE
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_BUILD_TYPE "${CMAKE_BUILD_TYPE}" CACHE STRING "Reset the configurations to what we need" FORCE)


#---------------------------------------------------------------------
#
# Function which sets the output directories based on the configuration
# and some base values
#
function ( SetOutputDirectories _target _objdir _libdir _bindir )
    set_target_properties(${_target} PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${_libdir}) # For .lib (both static and import) and .a
    set_target_properties(${_target} PROPERTIES ARCHIVE_OUTPUT_DIRECTORY_DEBUG ${_libdir}/Debug)
    set_target_properties(${_target} PROPERTIES ARCHIVE_OUTPUT_DIRECTORY_PROFILE ${_libdir}/Profile)
    set_target_properties(${_target} PROPERTIES ARCHIVE_OUTPUT_DIRECTORY_RELEASE ${_libdir}/Release)
    set_target_properties(${_target} PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${_bindir}) # For .so
    set_target_properties(${_target} PROPERTIES LIBRARY_OUTPUT_DIRECTORY_DEBUG ${_bindir}/Debug)
    set_target_properties(${_target} PROPERTIES LIBRARY_OUTPUT_DIRECTORY_PROFILE ${_bindir}/Profile)
    set_target_properties(${_target} PROPERTIES LIBRARY_OUTPUT_DIRECTORY_RELEASE ${_bindir}/Release)
    set_target_properties(${_target} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${_bindir}) # For .dll
    set_target_properties(${_target} PROPERTIES RUNTIME_OUTPUT_DIRECTORY_DEBUG ${_bindir}/Debug)
    set_target_properties(${_target} PROPERTIES RUNTIME_OUTPUT_DIRECTORY_PROFILE ${_bindir}/Profile)
    set_target_properties(${_target} PROPERTIES RUNTIME_OUTPUT_DIRECTORY_RELEASE ${_bindir}/Release)
endfunction ( SetOutputDirectories )
