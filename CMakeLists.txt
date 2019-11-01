cmake_minimum_required(VERSION 3.15)
cmake_policy(SET CMP0091 NEW)

project(pbrt-parsing-perf)

set(CMAKE_CXX_STANDARD 11)
if(MSVC)
  set(CMAKE_CXX_FLAGS "/EHsc")
#  set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
  set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded")
endif(MSVC)


#
# Include root for our own sources
#

include_directories(src)


#
# Include root for minipbrt
#

include_directories(3rdparty/minipbrt)


#
# Use Ingo Wald's pbrt-parser, building it as part of this project.
#

add_subdirectory(3rdparty/pbrt-parser)
include_directories(3rdparty/pbrt-parser/pbrtParser/include)


# 
# Header-only libraries
# 

include_directories(3rdparty/pieces)


# 
# Define our executable
# 

add_executable(pbrt-parsing-perf
  src/main.cpp

  # minipbrt source files
  3rdparty/minipbrt/minipbrt.cpp
  3rdparty/minipbrt/minipbrt.h

  # pieces source files
  3rdparty/pieces/vh_cmdline.cpp
  3rdparty/pieces/vh_cmdline.h
  3rdparty/pieces/vh_time.cpp
  3rdparty/pieces/vh_time.h
)
target_link_libraries(pbrt-parsing-perf pbrtParser)
target_compile_definitions(pbrt-parsing-perf PRIVATE -D_USE_MATH_DEFINES)
