cmake_minimum_required(VERSION 2.8)

project(multi_array)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Werror -std=c++11 -Wpedantic -O3")

enable_testing()

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

include_directories(src)

set(INDEX_CC_FILES "${INDEX_CC_FILES}" "src/ranges/range_base.cc")
set(MA_CC_FILES "${MA_CC_FILES}" "${INDEX_CC_FILES}" "src/operation_utils.cc")

add_executable(iutest src/ranges/tests/index_unit_test.cc ${INDEX_CC_FILES})
target_link_libraries(iutest ${GTEST_BOTH_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT})
add_test(NAME iutest COMMAND iutest)

add_executable(mautest src/tests/ma_unit_test.cc ${MA_CC_FILES})
target_link_libraries(mautest ${GTEST_BOTH_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT})
add_test(NAME mautest COMMAND mautest)

add_executable(oputest src/tests/op_unit_test.cc ${MA_CC_FILES})
target_link_libraries(oputest ${GTEST_BOTH_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT})
add_test(NAME oputest COMMAND oputest)

add_executable(opptest src/tests/op_perf_test.cc ${MA_CC_FILES})
#target_link_libraries(oputest)

#install(TARGETS testm DESTINATION install)

set(CMAKE_INSTALL_PREFIX ..)
