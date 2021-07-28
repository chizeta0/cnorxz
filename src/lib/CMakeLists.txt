
execute_process ( COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/mk_hl_op.sh
  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/ )

set(libcnorxz_a_SOURCES
  ${CMAKE_SOURCE_DIR}/src/lib/ranges/range_base.cc
  ${CMAKE_SOURCE_DIR}/src/lib/ranges/anonymous_range.cc
  ${CMAKE_SOURCE_DIR}/src/lib/ranges/dynamic_meta.cc
  ${CMAKE_SOURCE_DIR}/src/lib/ranges/dynamic_range.cc
  ${CMAKE_SOURCE_DIR}/src/lib/ranges/type_map.cc
  ${CMAKE_SOURCE_DIR}/src/lib/ranges/multi_range_factory_product_map.cc
  ${CMAKE_SOURCE_DIR}/src/lib/map_range_factory_product_map.cc
  )

set(libhlcnorxz_a_SOURCES
  ${CMAKE_SOURCE_DIR}/src/lib/high_level_operation.cc
  )
#message(WARNING ${libhlcnorxz_a_SOURCES})

file(GLOB cc_files "${CMAKE_SOURCE_DIR}/src/lib/ranges/range_types/*.cc")
foreach(ccfile ${cc_files})
  set(libcnorxz_a_SOURCES ${libcnorxz_a_SOURCES}
    ${ccfile})
endforeach(ccfile)

file(GLOB cc_files "${CMAKE_SOURCE_DIR}/src/lib/hl_ops/*.cc")
foreach(ccfile ${cc_files})
  set(libhlcnorxz_a_SOURCES ${libhlcnorxz_a_SOURCES}
    ${ccfile})
endforeach(ccfile)

add_library(cnorxz_obj OBJECT
  ${libcnorxz_a_SOURCES}
  )
add_library(hlcnorxz_obj OBJECT
  ${libhlcnorxz_a_SOURCES}
  )
set_target_properties(cnorxz_obj PROPERTIES POSITION_INDEPENDENT_CODE TRUE)
set_target_properties(hlcnorxz_obj PROPERTIES POSITION_INDEPENDENT_CODE TRUE)

add_library(cnorxz SHARED
  $<TARGET_OBJECTS:cnorxz_obj>
  )
set_target_properties(cnorxz PROPERTIES POSITION_INDEPENDENT_CODE TRUE)

add_library(cnorxz_static
  $<TARGET_OBJECTS:cnorxz_obj>
  )
set_target_properties(cnorxz_obj PROPERTIES POSITION_INDEPENDENT_CODE TRUE)

add_library(hlcnorxz SHARED
  $<TARGET_OBJECTS:hlcnorxz_obj>
  )
set_target_properties(hlcnorxz PROPERTIES POSITION_INDEPENDENT_CODE TRUE)

add_library(hlcnorxz_static
  $<TARGET_OBJECTS:hlcnorxz_obj>
  )
set_target_properties(hlcnorxz_obj PROPERTIES POSITION_INDEPENDENT_CODE TRUE)

install(TARGETS cnorxz
  ARCHIVE DESTINATION ${INSTALL_PATH}/lib
  LIBRARY DESTINATION ${INSTALL_PATH}/lib)

install(TARGETS cnorxz_static
  ARCHIVE DESTINATION ${INSTALL_PATH}/lib
  LIBRARY DESTINATION ${INSTALL_PATH}/lib)

install(TARGETS hlcnorxz
  ARCHIVE DESTINATION ${INSTALL_PATH}/lib
  LIBRARY DESTINATION ${INSTALL_PATH}/lib)

install(TARGETS hlcnorxz_static
  ARCHIVE DESTINATION ${INSTALL_PATH}/lib
  LIBRARY DESTINATION ${INSTALL_PATH}/lib)
