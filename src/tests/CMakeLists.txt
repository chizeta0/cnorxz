
add_definitions(-DTEST_NUMBER_FILE="${CMAKE_SOURCE_DIR}/src/tests/numbers.txt")

set(test_SOURCES
  ${CMAKE_SOURCE_DIR}/src/tests/test_numbers.cc
)

add_library(test_lib STATIC ${test_SOURCES})

add_executable(xprutest xpr_unit_test.cc)
add_dependencies(xprutest cnorxz)
target_link_libraries(xprutest ${GTEST_BOTH_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT} cnorxz test_lib)
add_test(NAME xprutest COMMAND xprutest)

add_executable(rutest range_unit_test.cc)
add_dependencies(rutest cnorxz)
target_link_libraries(rutest ${GTEST_BOTH_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT} cnorxz test_lib)
add_test(NAME rutest COMMAND rutest)

add_executable(dautest darray_unit_test.cc)
add_dependencies(dautest cnorxz)
target_link_libraries(dautest ${GTEST_BOTH_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT} cnorxz test_lib)
add_test(NAME dautest COMMAND dautest)

add_executable(oputest operation_unit_test.cc)
add_dependencies(oputest cnorxz)
target_link_libraries(oputest ${GTEST_BOTH_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT} cnorxz test_lib)
add_test(NAME oputest COMMAND oputest)

#add_executable(opptest op_perf_test.cc)
#add_dependencies(opptest cnorxz)
#target_link_libraries(opptest cnorxz)
