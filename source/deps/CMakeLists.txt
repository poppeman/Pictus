set(DEPS_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
set(DEPS_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PARENT_SCOPE)

add_subdirectory (libwebp)
add_subdirectory (orz)
add_subdirectory (Metadata)
add_subdirectory (illa)
add_subdirectory (UnitTest++)
