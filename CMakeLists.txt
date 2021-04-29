cmake_minimum_required(VERSION 3.17)
project(nokia_csv_reader)

set(CMAKE_CXX_STANDARD 17)

add_executable(nokia_csv_reader src/main.cpp src/csv_reader.cpp)

find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

add_executable(nokia_csv_test test/test.cpp src/csv_reader.cpp)
target_link_libraries(nokia_csv_test ${GTEST_LIBRARIES} pthread)
