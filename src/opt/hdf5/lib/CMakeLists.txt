
set(libcnorxzhdf5_a_SOURCES
  ${CMAKE_CURRENT_SOURCE_DIR}/h5_content_base.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/h5_file.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/h5_group.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/h5_table.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/h5_dataset.cc
  )

add_library(cnorxzhdf5_obj OBJECT
  ${libcnorxzhdf5_a_SOURCES}
  )

set_target_properties(cnorxzhdf5_obj PROPERTIES POSITION_INDEPENDENT_CODE TRUE)

add_library(cnorxzhdf5 SHARED
  $<TARGET_OBJECTS:cnorxzhdf5_obj>
  )
set_target_properties(cnorxzhdf5 PROPERTIES POSITION_INDEPENDENT_CODE TRUE)

install(TARGETS cnorxzhdf5 LIBRARY DESTINATION ${INSTALL_PATH}/lib)
