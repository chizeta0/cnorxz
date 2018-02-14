
#add_executable(iutest /ranges/index_unit_test.cc ${INDEX_CC_FILES})
add_executable(iutest ranges/index_unit_test.cc)
add_dependencies(iutest multiarray)
target_link_libraries(iutest ${GTEST_BOTH_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT} multiarray)
add_test(NAME iutest COMMAND iutest)

#add_executable(mautest src/tests/ma_unit_test.cc ${MA_CC_FILES})
add_executable(mautest ma_unit_test.cc)
add_dependencies(mautest multiarray)
target_link_libraries(mautest ${GTEST_BOTH_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT} multiarray)
add_test(NAME mautest COMMAND mautest)

add_executable(oputest op_unit_test.cc)
add_dependencies(oputest multiarray)
target_link_libraries(oputest ${GTEST_BOTH_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT} multiarray)
add_test(NAME oputest COMMAND oputest)

add_executable(opptest op_perf_test.cc)
add_dependencies(opptest multiarray)
target_link_libraries(opptest multiarray)
