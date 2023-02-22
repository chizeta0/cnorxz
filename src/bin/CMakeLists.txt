
add_executable(cnorxz-config config.cc)
add_dependencies(cnorxz-config cnorxz)
target_link_libraries(cnorxz-config cnorxz)

install(TARGETS cnorxz-config RUNTIME DESTINATION ${INSTALL_PATH}/bin)
