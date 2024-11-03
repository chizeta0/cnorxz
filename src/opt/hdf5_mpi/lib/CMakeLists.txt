
set(libcnorxzhdf5mpi_a_SOURCES
  ${CMAKE_CURRENT_SOURCE_DIR}/h5_rfile.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/h5_rdataset.cc
  )

add_library(cnorxzhdf5mpi_obj OBJECT
  ${libcnorxzhdf5mpi_a_SOURCES}
  )

set_target_properties(cnorxzhdf5mpi_obj PROPERTIES POSITION_INDEPENDENT_CODE TRUE)

add_library(cnorxzhdf5mpi SHARED
  $<TARGET_OBJECTS:cnorxzhdf5mpi_obj>
  )
set_target_properties(cnorxzhdf5mpi PROPERTIES POSITION_INDEPENDENT_CODE TRUE)

install(TARGETS cnorxzhdf5mpi LIBRARY DESTINATION ${INSTALL_PATH}/lib)
