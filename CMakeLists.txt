cmake_minimum_required(VERSION 2.8)

project(multi_array)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Werror -std=c++11 -Wpedantic -O3 -g -march=native -fopenmp")

enable_testing()

if(IS_ABSOLUTE "${CMAKE_INSTALL_PREFIX}")
  set(INSTALL_PATH ${CMAKE_INSTALL_PREFIX})
else()
  #set(INSTALL_PATH ${CMAKE_SOURCE_DIR}/install)
  get_filename_component(INSTALL_PATH ${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_PREFIX} ABSOLUTE)
endif()
message(WARNING "found absolute install path '${INSTALL_PATH}'") 

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
