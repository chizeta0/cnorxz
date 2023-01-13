
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

add_subdirectory(lib)
#add_subdirectory(tests)

set(HDF5_INSTALL_PATH ${INSTALL_PATH}/hdf5)
install(DIRECTORY include/ DESTINATION ${HDF5_INSTALL_PATH}/include)
