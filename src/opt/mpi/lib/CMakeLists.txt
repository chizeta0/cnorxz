
set(libcnorxzmpi_a_SOURCES
  ${CMAKE_CURRENT_SOURCE_DIR}/mpi_base.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/rrange.cc
  )

add_library(cnorxzmpi_obj OBJECT
  ${libcnorxzmpi_a_SOURCES}
  )

set_target_properties(cnorxzmpi_obj PROPERTIES POSITION_INDEPENDENT_CODE TRUE)

add_library(cnorxzmpi SHARED
  $<TARGET_OBJECTS:cnorxzmpi_obj>
  )
set_target_properties(cnorxzmpi PROPERTIES POSITION_INDEPENDENT_CODE TRUE)

install(TARGETS cnorxzmpi LIBRARY DESTINATION ${INSTALL_PATH}/lib)
