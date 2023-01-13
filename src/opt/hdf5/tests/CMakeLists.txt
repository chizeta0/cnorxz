
set(TEST_FILE_BASE "${CMAKE_CURRENT_BINARY_DIR}/test-file-")
message(STATUS "TEST_FILE_BASE = ${TEST_FILE_BASE}")
add_definitions(-DTEST_FILE_BASE=${TEST_FILE_BASE})

#add_test(NAME clean COMMAND rm -f ${TEST_FILE_BASE}*)

add_executable(h5basic h5_basic_unit_test.cc)
add_dependencies(h5basic cnorxz cnorxzhdf5)
target_link_libraries(h5basic ${GTEST_BOTH_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT} ${HDF5_LIBS} cnorxz cnorxzhdf5)
add_test(NAME h5basic COMMAND rm -f ${TEST_FILE_BASE}* ; h5basic)
