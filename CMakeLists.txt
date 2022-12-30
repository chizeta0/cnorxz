cmake_minimum_required(VERSION 3.0)
  
project(cnorxz)

if(CMAKE_COMPILER_IS_GNUCXX)
  if(NOT CMAKE_CXX_COMPILER_VERSION VERSION_GREATER_EQUAL 7.0)
    message(FATAL_ERROR "require gcc version >= 7.0")
  endif()
else()
  message(WARNING "compiler ${CMAKE_CXX_COMPILER_ID} officially not supported")
endif()

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Werror -std=c++17 -Wpedantic -O2 -march=native -faligned-new -funroll-loops -fopenmp")

enable_testing()

if(IS_ABSOLUTE "${CMAKE_INSTALL_PREFIX}")
  set(INSTALL_PATH ${CMAKE_INSTALL_PREFIX})
else()
  #set(INSTALL_PATH ${CMAKE_SOURCE_DIR}/install)
  get_filename_component(INSTALL_PATH ${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_PREFIX} ABSOLUTE)
endif()
message(STATUS "found absolute install path '${INSTALL_PATH}'") 

find_package( GTest REQUIRED )
if(GTest_FOUND)
  include_directories(${GTEST_INCLUDE_DIRS})
else()
  message(FATAL_ERROR "GTest not found")
endif()

find_package(Threads REQUIRED)
if(Threads_FOUND)
  #include_directories(${Threads_INCLUDE_DIRS})
else()
  message(FATAL_ERROR "Threads not found")
endif()

add_subdirectory(src)
