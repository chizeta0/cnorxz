
add_executable(cnorxz-config config.cc)
add_dependencies(cnorxz-config cnorxz)
target_link_libraries(cnorxz-config cnorxz)

install(TARGETS cnorxz-config RUNTIME DESTINATION ${INSTALL_PATH}/bin)

#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")

add_executable(performance performance.cc)
add_dependencies(performance cnorxz)
target_link_libraries(performance cnorxz)

install(TARGETS performance RUNTIME DESTINATION ${INSTALL_PATH}/bin)
