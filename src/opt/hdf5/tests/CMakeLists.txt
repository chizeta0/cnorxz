
add_executable(h5basic h5_basic_unit_test.cc)
add_dependencies(h5basic cnorxz cnorxzhdf5)
target_link_libraries(h5basic ${GTEST_BOTH_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT} ${HDF5_LIBS} cnorxz cnorxzhdf5)
add_test(NAME h5basic COMMAND h5basic)
