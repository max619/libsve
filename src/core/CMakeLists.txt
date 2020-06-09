set(CORE_SOURCES
src/BitmapFrame.cpp
src/Frame.cpp
src/PixelFormats.cpp
src/PixelFormatsConverter.cpp
src/Utils.cpp
src/Utils.h
include/BitmapFrame.h
include/Frame.h
include/PixelFormats.h
include/PixelFormatsConverter.h)

#install(FILES ${CMAKE_CURRENT_BINARY_DIR}/include/OpenShotVersion.h
#       DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/libopenshot)


add_library(core STATIC)

target_include_directories(core 
PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

target_include_directories(core 
PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)

target_sources(core PRIVATE 
${CORE_SOURCES})