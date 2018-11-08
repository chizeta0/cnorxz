
set(libmultiarray_a_SOURCES
  ${CMAKE_SOURCE_DIR}/src/lib/ranges/range_base.cc
  ${CMAKE_SOURCE_DIR}/src/lib/ranges/anonymous_range.cc
  ${CMAKE_SOURCE_DIR}/src/lib/ranges/dynamic_meta.cc
  ${CMAKE_SOURCE_DIR}/src/lib/ranges/multi_range_factory_product_map.cc
  ${CMAKE_SOURCE_DIR}/src/lib/map_range_factory_product_map.cc
  )

file(GLOB cc_files "${CMAKE_SOURCE_DIR}/src/lib/ranges/range_types/*.cc")
foreach(ccfile ${cc_files})
  set(libmultiarray_a_SOURCES ${libmultiarray_a_SOURCES}
    ${ccfile})
endforeach(ccfile)

add_library(multiarray SHARED ${libmultiarray_a_SOURCES})
install(TARGETS multiarray
  ARCHIVE DESTINATION ${INSTALL_PATH}/lib
  LIBRARY DESTINATION ${INSTALL_PATH}/lib)
