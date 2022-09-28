
add_definitions(-DTEST_NUMBER_FILE="${CMAKE_SOURCE_DIR}/src/tests/numbers.txt")

set(test_SOURCES
  ${CMAKE_SOURCE_DIR}/src/tests/test_numbers.cc
)

add_library(test_lib STATIC ${test_SOURCES})

#add_executable(iutest ranges/index_unit_test.cc)
#add_dependencies(iutest cnorxz)
#target_link_libraries(iutest ${GTEST_BOTH_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT} cnorxz)
#add_test(NAME iutest COMMAND iutest)

add_executable(xfutest xfor_unit_test.cc)
add_dependencies(xfutest cnorxz)
target_link_libraries(xfutest ${GTEST_BOTH_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT} cnorxz test_lib)
add_test(NAME xfutest COMMAND xfutest)

add_executable(rutest range_unit_test.cc)
add_dependencies(rutest cnorxz)
target_link_libraries(rutest ${GTEST_BOTH_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT} cnorxz test_lib)
add_test(NAME rutest COMMAND rutest)

#add_executable(autest ranges/anonymous_unit_test.cc)
#add_dependencies(autest cnorxz)
#target_link_libraries(autest ${GTEST_BOTH_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT} cnorxz)
#add_test(NAME autest COMMAND autest)

#add_executable(mautest array_unit_test.cc)
#add_dependencies(mautest cnorxz)
#target_link_libraries(mautest ${GTEST_BOTH_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT} cnorxz)
#add_test(NAME mautest COMMAND mautest)

add_executable(dautest darray_unit_test.cc)
add_dependencies(dautest cnorxz)
target_link_libraries(dautest ${GTEST_BOTH_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT} cnorxz test_lib)
add_test(NAME dautest COMMAND dautest)

#add_executable(oputest op_unit_test.cc)
#add_dependencies(oputest cnorxz)
#target_link_libraries(oputest ${GTEST_BOTH_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT} cnorxz)
#add_test(NAME oputest COMMAND oputest)

#add_executable(op2utest op2_unit_test.cc)
#add_dependencies(op2utest cnorxz)
#target_link_libraries(op2utest ${GTEST_BOTH_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT} cnorxz)
#add_test(NAME op2utest COMMAND op2utest)

#add_executable(op3utest op3_unit_test.cc)
#add_dependencies(op3utest cnorxz)
#target_link_libraries(op3utest ${GTEST_BOTH_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT} cnorxz)
#add_test(NAME op3utest COMMAND op3utest)

#add_executable(op4utest op4_unit_test.cc)
#add_dependencies(op4utest cnorxz hlcnorxz)
#target_link_libraries(op4utest ${GTEST_BOTH_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT} cnorxz hlcnorxz)
#add_test(NAME op4utest COMMAND op4utest)

#add_executable(opptest op_perf_test.cc)
#add_dependencies(opptest cnorxz)
#target_link_libraries(opptest cnorxz)
