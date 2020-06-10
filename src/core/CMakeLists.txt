set(CORE_SOURCES
src/BitmapFrame.cpp
src/Frame.cpp
src/PixelFormats.cpp
src/PixelFormatsConverter.cpp
src/GPUContext.cpp
src/Utils.cpp
src/Utils.h
src/PrivateGPUContextStructsImpl.h
src/GPUKernel.cpp
src/GPUKernelSourceProvider.cpp
include/BitmapFrame.h
include/PImpl.h
include/GPUContext.h
include/Frame.h
include/PixelFormats.h
include/PixelFormatsConverter.h
include/GPUKernel.h
include/GPUKernelSourceProvider.h)

#install(FILES ${CMAKE_CURRENT_BINARY_DIR}/include/OpenShotVersion.h
#       DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/libopenshot)

if(!OPENCL_FOUND)
	error( "OPENCL Not found" )

endif()

add_library(core STATIC)

target_include_directories(core 
PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

target_include_directories(core PRIVATE ${OpenCL_INCLUDE_DIRS})
target_link_directories(core PUBLIC ${OpenCL_LIBRARY})

target_link_libraries (core PUBLIC ${OpenCL_LIBRARY})

target_include_directories(core 
PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)


target_sources(core PRIVATE 
${CORE_SOURCES})