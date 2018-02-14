cmake_minimum_required(VERSION 2.8)

project(multi_array)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Werror -std=c++11 -Wpedantic -Wno-uninitialized -O3")

enable_testing()

set(INSTALL_PATH ${CMAKE_SOURCE_DIR}/install)

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
