
add_definitions(-DTEST_NUMBER_FILE="${CMAKE_SOURCE_DIR}/src/tests/numbers.txt")
include_directories(${CMAKE_SOURCE_DIR}/src/tests)

add_executable(mpirrutest rrange_unit_test.cc)
add_dependencies(mpirrutest cnorxz cnorxzmpi test_lib)
target_link_libraries(mpirrutest ${GTEST_BOTH_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT} ${MPI_LIBS} cnorxz cnorxzmpi test_lib)
set(MPI_TEST_COMMAND mpirun -n 4 mpirrutest)
add_test(NAME mpirrutest COMMAND ${MPI_TEST_COMMAND})

add_executable(mpirautest rarray_unit_test.cc)
add_dependencies(mpirautest cnorxz cnorxzmpi test_lib)
target_link_libraries(mpirautest ${GTEST_BOTH_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT} ${MPI_LIBS} cnorxz cnorxzmpi test_lib)
set(MPI_TEST_COMMAND mpirun -n 4 mpirautest)
add_test(NAME mpirautest COMMAND ${MPI_TEST_COMMAND})

add_executable(mpisbutest setbuf_unit_test.cc)
add_dependencies(mpisbutest cnorxz cnorxzmpi test_lib)
target_link_libraries(mpisbutest ${GTEST_BOTH_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT} ${MPI_LIBS} cnorxz cnorxzmpi test_lib)
set(MPI_TEST_COMMAND mpirun -n 4 mpisbutest)
add_test(NAME mpisbutest COMMAND ${MPI_TEST_COMMAND})
