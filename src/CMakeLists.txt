cmake_minimum_required(VERSION 3.2)
project(lab2)

set(SOURCE_FILES main.cpp decoder.cpp)

add_executable(lab2 ${SOURCE_FILES})
install(TARGETS lab2 DESTINATION ${INSTALL_BIN_DIR})
