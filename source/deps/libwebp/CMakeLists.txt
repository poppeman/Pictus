cmake_minimum_required (VERSION 2.8)

ADD_DEFINITIONS(-DNDEBUG -DWEBP_USE_THREAD)
add_subdirectory (src)



#add_library(libwebpmux STATIC ${WEBP_MUX_SRCS})
#target_link_libraries(libwebpmux m pthread)

