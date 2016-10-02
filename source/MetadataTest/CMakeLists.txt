cmake_minimum_required (VERSION 2.8)

SET(
	CPP_SRCS
	main.cpp
	test_exif_parser.cpp)
SET(
	H_SRCS
	main.h)

include_directories(${DEPS_DIRECTORY})

add_executable(MetadataTest ${CPP_SRCS} ${H_SRCS})
target_link_libraries(MetadataTest orz UnitTestPP Metadata ${Boost_LIBRARIES})
