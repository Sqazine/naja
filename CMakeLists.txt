cmake_minimum_required(VERSION 3.10)

project(najac LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_EXTENSION OFF)

add_subdirectory(libnajac)
add_subdirectory(tools)


add_executable(najac najac.cpp)
target_include_directories(najac ${CMAKE_SOURCE_DIR}/libnajac)
target_link_libraries(najac libnajac)