cmake_minimum_required(VERSION 3.10)

project(NajaLang LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_EXTENSION OFF)

file(GLOB NAJA_LANG_SRC "NajaLang/*.h" "NajaLang/*.cpp")
add_library(libNajaLang ${NAJA_LANG_SRC})

add_executable(naja Naja.cpp)
target_include_directories(naja PUBLIC ${CMAKE_SOURCE_DIR}/NajaLang)
target_link_libraries(naja PUBLIC libNajaLang)


add_executable(Rlpl ${CMAKE_SOURCE_DIR}/Tools/Rlpl.cpp)
target_include_directories(Rlpl PUBLIC ${CMAKE_SOURCE_DIR}/NajaLang)
target_link_libraries(Rlpl PUBLIC libNajaLang)

add_executable(Rppl ${CMAKE_SOURCE_DIR}/Tools/Rppl.cpp)
target_include_directories(Rppl PUBLIC ${CMAKE_SOURCE_DIR}/NajaLang)
target_link_libraries(Rppl PUBLIC libNajaLang)

add_executable(Rcpl ${CMAKE_SOURCE_DIR}/Tools/Rcpl.cpp)
target_include_directories(Rcpl PUBLIC ${CMAKE_SOURCE_DIR}/NajaLang)
target_link_libraries(Rcpl PUBLIC libNajaLang)
