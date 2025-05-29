cmake_minimum_required(VERSION 3.29)
project(CPP_20)

set(CMAKE_CXX_STANDARD 20)
#set(CMAKE_CXX_STANDARD_REQUIRED ON)
#set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(CPP_20 main.cpp
        Matrix.hpp
        #        Employee.cpp
#        Employee.hpp
#        Database.cpp
#        Database.hpp
)
#target_sources(main PUBLIC main.cpp)
#target_sources(main PUBLIC FILE_SET all_modules TYPE CXX_MODULES FILES employee.cppm)
