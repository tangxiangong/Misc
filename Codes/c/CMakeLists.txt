cmake_minimum_required(VERSION 3.30)
project(c)

set(CMAKE_C_STANDARD 23)

include_directories(${PROJECT_SOURCE_DIR}/include)
aux_source_directory(${PROJECT_SOURCE_DIR}/src SRC)

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
add_executable(main main.c ${SRC})
