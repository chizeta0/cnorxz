cmake_minimum_required(VERSION 2.8)

project(multi_array)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Werror -std=c++11 --pedantic")

find_package( GTest REQUIRED )
if(GTest_FOUND)
  include_directories(${GTEST_INCLUDE_DIRS})
else()
  message(FATAL_ERROR "GTest not found")
endif()

include_directories(src)

add_executable(utest src/unit_test.cc)
target_link_libraries(utest ${GTEST_BOTH_LIBRARIES})
add_test(AllTests utest)

#install(TARGETS testm DESTINATION install)

set(CMAKE_INSTALL_PREFIX ..)