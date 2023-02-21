
set(libcnorxz_a_SOURCES
  ${CMAKE_SOURCE_DIR}/src/lib/base/to_string.cc
  ${CMAKE_SOURCE_DIR}/src/lib/base/uuid.cc
  ${CMAKE_SOURCE_DIR}/src/lib/memory/memcount.cc
  ${CMAKE_SOURCE_DIR}/src/lib/ranges/range_base.cc
  ${CMAKE_SOURCE_DIR}/src/lib/ranges/yrange.cc
  ${CMAKE_SOURCE_DIR}/src/lib/ranges/crange.cc
  ${CMAKE_SOURCE_DIR}/src/lib/ranges/dindex.cc
  ${CMAKE_SOURCE_DIR}/src/lib/ranges/index_pack.cc
  ${CMAKE_SOURCE_DIR}/src/lib/ranges/index_format.cc
  )

add_library(cnorxz_obj OBJECT
  ${libcnorxz_a_SOURCES}
  )

set_target_properties(cnorxz_obj PROPERTIES POSITION_INDEPENDENT_CODE TRUE)

add_library(cnorxz SHARED
  $<TARGET_OBJECTS:cnorxz_obj>
  )
set_target_properties(cnorxz PROPERTIES POSITION_INDEPENDENT_CODE TRUE)

#add_library(cnorxz_static
#  $<TARGET_OBJECTS:cnorxz_obj>
#  )
#set_target_properties(cnorxz_obj PROPERTIES POSITION_INDEPENDENT_CODE TRUE)

install(TARGETS cnorxz LIBRARY DESTINATION ${INSTALL_PATH}/lib)

#install(TARGETS cnorxz_static
#  ARCHIVE DESTINATION ${INSTALL_PATH}/lib
#  LIBRARY DESTINATION ${INSTALL_PATH}/lib)
