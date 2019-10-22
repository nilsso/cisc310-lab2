cmake_minimum_required(VERSION 3.2)

project(lab2)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -O3 -Wall -Wextra")

set(CMAKE_INSTALL_PREFIX ${PROJECT_SOURCE_DIR})

set(INSTALL_BIN_DIR ${PROJECT_SOURCE_DIR}/bin)
set(INSTALL_LIB_DIR ${PROJECT_SOURCE_DIR}/lib)
set(INSTALL_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/include)

set(HEADERS_DIR ${PROJECT_SOURCE_DIR}/src)

add_subdirectory(src)
add_subdirectory(test)
