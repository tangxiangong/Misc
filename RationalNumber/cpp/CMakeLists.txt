cmake_minimum_required(VERSION 3.30)
project(RationalNumber)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
# 启用实验性的模块支持
set(CMAKE_EXPERIMENTAL_CXX_MODULE_CMAKE_API "2182bf5c-ef0d-489a-91da-49dbc3090d2a")
set(CMAKE_EXPERIMENTAL_CXX_MODULE_DYNDEP ON)


# include_directories(${PROJECT_SOURCE_DIR}/include)
# aux_source_directory(${PROJECT_SOURCE_DIR}/src SRC)


# 声明模块库
add_library(rational)
target_sources(rational
        PUBLIC
        FILE_SET CXX_MODULES FILES modules/rational.cppm
)

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
add_executable(main main.cpp)
target_link_libraries(main rational)