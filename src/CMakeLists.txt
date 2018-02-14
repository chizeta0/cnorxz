
include_directories(${CMAKE_SOURCE_DIR}/src/include)
add_subdirectory(tests)
add_subdirectory(lib)

install(DIRECTORY include/ DESTINATION ${INSTALL_PATH}/include)
