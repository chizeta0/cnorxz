
execute_process ( COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/mk_hl_op.sh
  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/ )

set(libmultiarray_a_SOURCES
  ${CMAKE_SOURCE_DIR}/src/lib/ranges/range_base.cc
  ${CMAKE_SOURCE_DIR}/src/lib/ranges/anonymous_range.cc
  ${CMAKE_SOURCE_DIR}/src/lib/ranges/dynamic_meta.cc
  ${CMAKE_SOURCE_DIR}/src/lib/ranges/dynamic_range.cc
  ${CMAKE_SOURCE_DIR}/src/lib/ranges/type_map.cc
  ${CMAKE_SOURCE_DIR}/src/lib/ranges/multi_range_factory_product_map.cc
  ${CMAKE_SOURCE_DIR}/src/lib/map_range_factory_product_map.cc
  ${CMAKE_SOURCE_DIR}/src/lib/high_level_operation.cc
  )

file(GLOB cc_files "${CMAKE_SOURCE_DIR}/src/lib/ranges/range_types/*.cc")
foreach(ccfile ${cc_files})
  set(libmultiarray_a_SOURCES ${libmultiarray_a_SOURCES}
    ${ccfile})
endforeach(ccfile)

file(GLOB cc_files "${CMAKE_SOURCE_DIR}/src/lib/hl_ops/*.cc")
foreach(ccfile ${cc_files})
  set(libmultiarray_a_SOURCES ${libmultiarray_a_SOURCES}
    ${ccfile})
endforeach(ccfile)

add_library(multiarray_obj OBJECT
  ${libmultiarray_a_SOURCES}
  )
set_target_properties(multiarray_obj PROPERTIES POSITION_INDEPENDENT_CODE TRUE)

add_library(multiarray SHARED
  $<TARGET_OBJECTS:multiarray_obj>
  )
set_target_properties(multiarray PROPERTIES POSITION_INDEPENDENT_CODE TRUE)

add_library(multiarray_static
  $<TARGET_OBJECTS:multiarray_obj>
  )
set_target_properties(multiarray_obj PROPERTIES POSITION_INDEPENDENT_CODE TRUE)

install(TARGETS multiarray
  ARCHIVE DESTINATION ${INSTALL_PATH}/lib
  LIBRARY DESTINATION ${INSTALL_PATH}/lib)

install(TARGETS multiarray_static
  ARCHIVE DESTINATION ${INSTALL_PATH}/lib
  LIBRARY DESTINATION ${INSTALL_PATH}/lib)
