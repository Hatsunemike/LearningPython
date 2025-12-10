cmake_minimum_required(VERSION 3.10)
project(MyProject CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-Wall -Wextra -O2")
include_directories(PRIVATE include)

find_package(Boost REQUIRED COMPONENTS filesystem system random)

file(GLOB COMMON_SOURCES "src/*.cpp" "src/_*/*.cpp")

if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/src/tests")
    file(GLOB TEST_SOURCES "src/tests/*.cpp")
    add_executable(testmain
        ${COMMON_SOURCES}
        ${TEST_SOURCES}
    )
    target_link_libraries(testmain ssl crypto gmp)
endif()

if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/src/elgamel")
    file(GLOB ELGAMEL_SOURCES "src/elgamel/*.cpp")
    add_executable(elgamel
        ${COMMON_SOURCES}
        ${ELGAMEL_SOURCES}
    )
    target_link_libraries(elgamel ssl crypto gmp ${Boost_LIBRARIES})
endif()