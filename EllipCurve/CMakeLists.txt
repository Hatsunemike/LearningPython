cmake_minimum_required(VERSION 3.10)
project(MyProject CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-Wall -Wextra -O2")

file(GLOB_RECURSE SOURCES "src/*.cpp")

add_executable(myprogram ${SOURCES})
target_include_directories(myprogram PRIVATE include)
target_link_libraries(myprogram ssl crypto gmp)