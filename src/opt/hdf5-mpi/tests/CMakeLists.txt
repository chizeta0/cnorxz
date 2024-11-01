
add_definitions(-DTEST_NUMBER_FILE="${CMAKE_SOURCE_DIR}/src/tests/numbers.txt")
include_directories(${CMAKE_SOURCE_DIR}/src/tests)
include_directories(${CMAKE_SOURCE_DIR}/src/opt/mpi/tests)
add_executable(h5mpibasic h5_mpi_basic_unit_test.cc)
add_dependencies(h5mpibasic cnorxz cnorxzhdf5 test_lib)
target_link_libraries(h5mpibasic ${GTEST_BOTH_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT} ${HDF5_LIBS} ${MPI_LIBS} cnorxz cnorxzhdf5 cnorxzmpi cnorxzhdf5mpi test_lib)
set(MPI_TEST_COMMAND mpirun -n 4 h5mpibasic)
add_test(NAME h5mpibasic COMMAND ${MPI_TEST_COMMAND})
