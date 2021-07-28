
#add_executable(iutest /ranges/index_unit_test.cc ${INDEX_CC_FILES})
add_executable(iutest ranges/index_unit_test.cc)
add_dependencies(iutest multiarray)
target_link_libraries(iutest ${GTEST_BOTH_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT} multiarray)
add_test(NAME iutest COMMAND iutest)

add_executable(autest ranges/anonymous_unit_test.cc)
add_dependencies(autest multiarray)
target_link_libraries(autest ${GTEST_BOTH_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT} multiarray)
add_test(NAME autest COMMAND autest)

#add_executable(mautest src/tests/array_unit_test.cc ${MA_CC_FILES})
add_executable(mautest array_unit_test.cc)
add_dependencies(mautest multiarray)
target_link_libraries(mautest ${GTEST_BOTH_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT} multiarray)
add_test(NAME mautest COMMAND mautest)

add_executable(oputest op_unit_test.cc)
add_dependencies(oputest multiarray)
target_link_libraries(oputest ${GTEST_BOTH_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT} multiarray)
add_test(NAME oputest COMMAND oputest)

add_executable(op2utest op2_unit_test.cc)
add_dependencies(op2utest multiarray)
target_link_libraries(op2utest ${GTEST_BOTH_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT} multiarray)
add_test(NAME op2utest COMMAND op2utest)

add_executable(op3utest op3_unit_test.cc)
add_dependencies(op3utest multiarray)
target_link_libraries(op3utest ${GTEST_BOTH_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT} multiarray)
add_test(NAME op3utest COMMAND op3utest)

add_executable(op4utest op4_unit_test.cc)
add_dependencies(op4utest multiarray hlmultiarray)
target_link_libraries(op4utest ${GTEST_BOTH_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT} multiarray hlmultiarray)
add_test(NAME op4utest COMMAND op4utest)

add_executable(opptest op_perf_test.cc)
add_dependencies(opptest multiarray)
target_link_libraries(opptest multiarray)
