
add_definitions(-DTEST_NUMBER_FILE="${CMAKE_SOURCE_DIR}/src/tests/numbers.txt")
include_directories(${CMAKE_SOURCE_DIR}/src/tests)
add_executable(mpirrutest rrange_unit_test.cc)
add_dependencies(mpirrutest cnorxz cnorxzmpi test_lib)
target_link_libraries(mpirrutest ${GTEST_BOTH_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT} ${MPI_LIBS} cnorxz cnorxzmpi test_lib)
add_test(NAME mpirrutest COMMAND mpirrutest)
