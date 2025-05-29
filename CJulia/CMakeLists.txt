cmake_minimum_required(VERSION 3.26)
project(CJulia C)

set(CMAKE_C_STANDARD 11)
aux_source_directory(${PROJECT_SOURCE_DIR}/src SRCS)

# Julia 目录
#set(JULIA_INCLUDE /usr/local/include/julia/)
#set(JULIA_LIB /usr/local/lib/julia/libjulia.dylib)

include_directories(${PROJECT_SOURCE_DIR}/include)
#include_directories(${JULIA_INCLUDE} ./include)
#link_directories(${JULIA_LIB})

set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)

#add_library(test SHARED ${SRCS})
add_executable(CJulia ${SRCS} main.c)
