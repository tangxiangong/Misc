cmake_minimum_required(VERSION 3.30)
project(cpp)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(${PROJECT_SOURCE_DIR}/include)
aux_source_directory(${PROJECT_SOURCE_DIR}/src SRC)

# 启用实验性的模块支持
set(CMAKE_EXPERIMENTAL_CXX_MODULE_CMAKE_API "2182bf5c-ef0d-489a-91da-49dbc3090d2a")
set(CMAKE_EXPERIMENTAL_CXX_MODULE_DYNDEP ON)

file(GLOB MODULE_FILES "${PROJECT_SOURCE_DIR}/modules/*.cppm")

set(MODULE_LIBS "")

foreach (MODULE_FILE ${MODULE_FILES})
    get_filename_component(MODULE_NAME ${MODULE_FILE} NAME_WE)
    add_library(${MODULE_NAME})
    target_sources(${MODULE_NAME}
            PUBLIC
            FILE_SET CXX_MODULES FILES modules/${MODULE_NAME}.cppm
    )
    list(APPEND MODULE_LIBS ${MODULE_NAME})
endforeach ()

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
add_executable(main main.cpp ${SRC})
target_link_libraries(main ${MODULE_LIBS})
