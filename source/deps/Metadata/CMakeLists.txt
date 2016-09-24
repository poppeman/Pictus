cmake_minimum_required (VERSION 2.8)

set(Meta_SOURCES
	Exif_Converter.cpp
	Exif_IO.cpp
	Exif_Parser.cpp
	Exif_ReadIfd.cpp
	Metadata_Document.cpp
	Metadata_Value.cpp)

add_library(Metadata STATIC ${Meta_SOURCES})
target_link_libraries(Metadata orz ${Boost_LIBRARIES})