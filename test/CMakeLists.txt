cmake_minimum_required(VERSION 3.2)

add_subdirectory(lib/googletest)

project(lab2_tests)

include_directories(${HEADERS_DIR})
include_directories(lib/googletest/googletest/include)

set(SOURCE_FILES main.cpp src/queue.cpp)

add_executable(lab2_tests ${SOURCE_FILES})
target_link_libraries(lab2_tests gtest)
install(TARGETS lab2_tests DESTINATION bin)
