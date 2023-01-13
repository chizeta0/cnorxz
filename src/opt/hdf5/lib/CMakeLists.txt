
set(libcnorxzhdf5_a_SOURCES
  ${CMAKE_CURRENT_SOURCE_DIR}/h5_content_base.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/h5_file.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/h5_group.cc
  )

add_library(cnorxzhdf5_obj OBJECT
  ${libcnorxzhdf5_a_SOURCES}
  )

set_target_properties(cnorxzhdf5_obj PROPERTIES POSITION_INDEPENDENT_CODE TRUE)

add_library(cnorxzhdf5 SHARED
  $<TARGET_OBJECTS:cnorxzhdf5_obj>
  )
set_target_properties(cnorxzhdf5 PROPERTIES POSITION_INDEPENDENT_CODE TRUE)

install(TARGETS cnorxz
  ARCHIVE DESTINATION ${HDF5_INSTALL_PATH}/lib
  LIBRARY DESTINATION ${HDF5_INSTALL_PATH}/lib
  )
