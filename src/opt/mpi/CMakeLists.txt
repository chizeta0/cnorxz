
find_package(MPI REQUIRED)
if(MPI_FOUND)
  include_directories(${MPI_C_INCLUDE_DIRS})
else()
  message(FATAL_ERROR "HDF5 not found")
endif()
message(STATUS "mpi lib = ${MPI_C_LIBRARIES}")
set(MPI_LIBS ${MPI_LIBRARIES})

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

add_subdirectory(lib)
add_subdirectory(tests)

install(DIRECTORY include/ DESTINATION ${INSTALL_PATH}/include/cnorxz/mpi)
