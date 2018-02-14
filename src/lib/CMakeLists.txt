
set(libmultiarray_a_SOURCES
  ${CMAKE_SOURCE_DIR}/src/lib/ranges/range_base.cc
  ${CMAKE_SOURCE_DIR}/src/lib/ranges/index_info.cc
  #${CMAKE_SOURCE_DIR}/src/lib/operation_utils.cc
  )

add_library(multiarray STATIC ${libmultiarray_a_SOURCES})
install(TARGETS multiarray
  ARCHIVE DESTINATION ${INSTALL_PATH}/lib
  LIBRARY DESTINATION ${INSTALL_PATH}/lib)
