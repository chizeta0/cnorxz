
find_package(HDF5 REQUIRED COMPONENTS C HL)
if(HDF5_FOUND)
  include_directories(${HDF5_INCLUDE_DIRS})
else()
  message(FATAL_ERROR "HDF5 not found")
endif()
message(STATUS "hdf5 libs = ${HDF5_LIBRARIES}")
set(HDF5_LIBS ${HDF5_LIBRARIES} ${HDF5_HL_LIBRARIES})

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

add_subdirectory(lib)
add_subdirectory(tests)

set(HDF5_INSTALL_PATH ${INSTALL_PATH}/hdf5)
install(DIRECTORY include/ DESTINATION ${HDF5_INSTALL_PATH}/include)
