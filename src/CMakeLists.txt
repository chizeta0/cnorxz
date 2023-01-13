
include_directories(${CMAKE_SOURCE_DIR}/src/include)
add_subdirectory(tests)
add_subdirectory(lib)

if(ENABLE_hdf5)
  add_subdirectory(opt/hdf5)
endif()

install(DIRECTORY include/ DESTINATION ${INSTALL_PATH}/include)
