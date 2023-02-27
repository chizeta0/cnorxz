
add_executable(certest cereal_unit_test.cc)
add_dependencies(certest cnorxz)
target_link_libraries(certest ${GTEST_BOTH_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT} cnorxz)
add_test(NAME certest COMMAND certest)
