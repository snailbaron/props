cmake_minimum_required(VERSION 3.12)

project(props)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

option(PROPS_BUILD_TESTS_AND_EXAMPLES "Build tests and examples for props library" ON)

add_library(props INTERFACE)
target_include_directories(props INTERFACE include)

if(PROPS_BUILD_TESTS_AND_EXAMPLES)
    add_subdirectory(examples)
endif()
