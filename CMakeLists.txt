cmake_minimum_required(VERSION 3.2...3.14 FATAL_ERROR)

################ ADD CMAKE MODULES ##################
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/Modules")

project(video_ed LANGUAGES C CXX)


#### Enable C++11 (for std::shared_ptr support)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

IF (WIN32)
	SET_PROPERTY(GLOBAL PROPERTY WIN32 "WIN32")
ENDIF(WIN32)

find_package(OpenCL REQUIRED)

add_subdirectory(src)