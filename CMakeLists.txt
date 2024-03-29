cmake_minimum_required(VERSION 3.5)
  
project(cnorxz)

# LIB VERSION

set(V_MAJOR 0)
set(V_MINOR 0)
set(V_PATCH 0)
set(VERSION "${V_MAJOR}.${V_MINOR}.${V_PATCH}")

# OPTIONS

option(SCALAR_BUILD "" OFF)

# INCLUDES

include(cmake/check_avx.cmake)

# GIT VARIABLES

execute_process(COMMAND bash "-c" "git rev-parse HEAD" OUTPUT_VARIABLE GIT_HASH OUTPUT_STRIP_TRAILING_WHITESPACE)
execute_process(COMMAND bash "-c" "git rev-parse --abbrev-ref HEAD" OUTPUT_VARIABLE GIT_BRANCH OUTPUT_STRIP_TRAILING_WHITESPACE)

# BUILD VERSION STRING

message(STATUS "git hash = ${GIT_HASH}")
message(STATUS "git branch = ${GIT_BRANCH}")
if(NOT ("${GIT_BRANCH}" EQUAL "release"))
  string(SUBSTRING ${GIT_HASH} 0 7 GIT_HASH_SHORT)
  set(VERSION "${VERSION}-${GIT_BRANCH}-${GIT_HASH_SHORT}")
endif()
message(STATUS "version = ${VERSION}")

# CHECK COMPILER

if(CMAKE_COMPILER_IS_GNUCXX)
  if(NOT CMAKE_CXX_COMPILER_VERSION VERSION_GREATER_EQUAL 7.0)
    message(FATAL_ERROR "require gcc version >= 7.0")
  endif()
else()
  message(WARNING "compiler ${CMAKE_CXX_COMPILER_ID} officially not supported")
endif()

# FLAGS

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Werror -std=c++17 -Wpedantic -O2 -march=native -faligned-new -funroll-loops -fopenmp")

# TESTING

enable_testing()

# INSTALL PATH

if(IS_ABSOLUTE "${CMAKE_INSTALL_PREFIX}")
  set(INSTALL_PATH ${CMAKE_INSTALL_PREFIX})
else()
  #set(INSTALL_PATH ${CMAKE_SOURCE_DIR}/install)
  get_filename_component(INSTALL_PATH ${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_PREFIX} ABSOLUTE)
endif()
message(STATUS "found absolute install path '${INSTALL_PATH}'") 

# SCALAR / INTRINSICS

if(NOT ${SCALAR_BUILD})
  message(STATUS "check for intrinsics")
  check_avx()
  if(AVX_AVAIL)
    message(STATUS "AVX available")
    add_definitions("-DCXZ_HAVE_AVX")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -mavx")
  else()
    message(STATUS "AVX not available")
  endif()
endif()

# CHECK LIBRARIES : GTest

message(STATUS "check for libraries")
find_package( GTest REQUIRED )
if(GTest_FOUND)
  include_directories(${GTEST_INCLUDE_DIRS})
else()
  message(FATAL_ERROR "GTest not found")
endif()

# CHECK LIBRARIES : Threads

find_package(Threads REQUIRED)
if(Threads_FOUND)
  #include_directories(${Threads_INCLUDE_DIRS})
else()
  message(FATAL_ERROR "Threads not found")
endif()

# CHECK LIBRARIES : hdf5

if(DEFINED ENABLE_hdf5)
  set(ENABLE_hdf5 ${ENABLE_hdf5} CACHE BOOL "enable hdf5")
else()
  set(ENABLE_hdf5 TRUE CACHE BOOL "enable hdf5")
endif()

# CHECK LIBRARIES : cereal

if(DEFINED ENABLE_cereal)
  set(ENABLE_cereal ${ENABLE_cereal} CACHE BOOL "enable hdf5")
else()
  set(ENABLE_cereal TRUE CACHE BOOL "enable hdf5")
endif()

find_package(cereal QUIET)
if(cereal_FOUND)
  message(STATUS "found cereal")
  if(ENABLE_cereal)
    message(STATUS "enable cereal")
    add_definitions(-DHAVE_CEREAL)
  endif()
else()
  message(STATUS "no cereal")
  if(ENABLE_cereal)
    message(FATAL_ERROR "cereal has been enabled but the cereal library has not been found")
  endif()
endif()

# DEFINES

add_definitions(-DVERSION="${VERSION}")
add_definitions(-DGIT_COMMIT="${GIT_HASH}")
add_definitions(-DCXX_FLAGS="${CMAKE_CXX_FLAGS}")

# CONTINUE WITH SUB-DIRECTORIES

add_subdirectory(src)
