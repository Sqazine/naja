cmake_minimum_required(VERSION 3.10)

project(NajaLang LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_EXTENSION OFF)

file(GLOB NAJA_LANG_SRC ${CMAKE_SOURCE_DIR}/NajaLang/*.h ${CMAKE_SOURCE_DIR}/NajaLang/*.cpp)
source_group("NajaLang" NAJA_LANG_SRC)
add_library(libNajaLang ${NAJA_LANG_SRC})

add_executable(naja Naja.cpp)
target_include_directories(naja PUBLIC ${CMAKE_SOURCE_DIR}/NajaLang)
target_link_libraries(naja PUBLIC libNajaLang)

add_executable(rlpl ${CMAKE_SOURCE_DIR}/Tools/Rlpl.cpp)
target_include_directories(rlpl PUBLIC ${CMAKE_SOURCE_DIR}/NajaLang)
target_link_libraries(rlpl PUBLIC libNajaLang)

add_executable(rppl ${CMAKE_SOURCE_DIR}/Tools/Rppl.cpp)
target_include_directories(rppl PUBLIC ${CMAKE_SOURCE_DIR}/NajaLang)
target_link_libraries(rppl PUBLIC libNajaLang)

add_executable(rcpl ${CMAKE_SOURCE_DIR}/Tools/Rcpl.cpp)
target_include_directories(rcpl PUBLIC ${CMAKE_SOURCE_DIR}/NajaLang)
target_link_libraries(rcpl PUBLIC libNajaLang)
