
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

add_subdirectory(tests)

install(DIRECTORY include/ DESTINATION ${INSTALL_PATH}/include/cnorxz/cereal)
