# XrdClProxyRW plug-in - An XRootD client plug-in

set( VERSION_MAJOR "0" )
set( VERSION_MINOR "1" )
set( VERSION_PATCH "0")
set( RELEASE       "1")

set( PLUGIN_VERSION "4" )

cmake_minimum_required( VERSION 2.6 )
project( xrootd-client-plugin-radosfs )

set( CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/src ${PROJECT_SOURCE_DIR}/cmake
     ${PROJECT_SOURCE_DIR}/test )

if ( CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT )
  set( CMAKE_INSTALL_PREFIX "/usr" CACHE PATH "Default install path" FORCE )
  set ( CMAKE_INSTALL_SYSCONFDIR "/etc/" CACHE PATH "Default etc path" FORCE )
endif ( CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)

if( MacOSX )
  set( LIB_INSTALL_DIR lib )
else( MacOSX )
  set( LIB_INSTALL_DIR lib64 )
endif( MacOSX )

# include GNU path settings
include (GNUInstallDirs)

# This is so we only build the targets for creating the
# tarball and rpm packages (helpful for the build system)
set( NO_SOURCE CACHE BOOL false )

if( NOT NO_SOURCE )
  if ( "${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU" OR
       "${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang" )
    set( WARNING_LEVEL "-Wall -Wno-unused-local-typedefs" )
    set( CMAKE_CXX_FLAGS ${WARNING_LEVEL}
         CACHE STRING "C++ compiler flags." FORCE )
    set( CMAKE_C_FLAGS  ${WARNING_LEVEL}
         CACHE STRING "C compiler flags." FORCE )
  endif()

  add_subdirectory( src )
  add_subdirectory( etc )

endif( NOT NO_SOURCE )

#-------------------------------------------------------------------------------
# 'uninstall' target
#-------------------------------------------------------------------------------

CONFIGURE_FILE(
  "${CMAKE_CURRENT_SOURCE_DIR}/cmake/cmake_uninstall.cmake.in"
  "${CMAKE_CURRENT_BINARY_DIR}/cmake/cmake_uninstall.cmake"
  IMMEDIATE @ONLY)

ADD_CUSTOM_TARGET(uninstall
  "${CMAKE_COMMAND}" -P "${CMAKE_CURRENT_BINARY_DIR}/cmake/cmake_uninstall.cmake")

#-------------------------------------------------------------------------------
# 'gitdist' target
#-------------------------------------------------------------------------------
add_custom_target(gitdist
    COMMAND git archive --prefix=${CMAKE_PROJECT_NAME}-${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}-${RELEASE}/ HEAD
        | gzip > ${CMAKE_BINARY_DIR}/${CMAKE_PROJECT_NAME}-${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}-${RELEASE}.tar.gz
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})

#-------------------------------------------------------------------------------
# 'srpm' target
#-------------------------------------------------------------------------------
ADD_CUSTOM_TARGET(srpm COMMAND rpmbuild -ts ${CMAKE_PROJECT_NAME}-${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}-${RELEASE}.tar.gz)
ADD_DEPENDENCIES(srpm distcheck)

#-------------------------------------------------------------------------------
# 'rpm' target
#-------------------------------------------------------------------------------
ADD_CUSTOM_TARGET(rpm COMMAND rpmbuild -tb ${CMAKE_PROJECT_NAME}-${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}-${RELEASE}.tar.gz)
ADD_DEPENDENCIES(rpm distcheck)

message( STATUS "Installation prefix: ${CMAKE_INSTALL_PREFIX}" )
