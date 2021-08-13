add_executable(rlpl rlpl.cpp)
target_include_directories(rlpl PUBLIC ${CMAKE_SOURCE_DIR}/libnajac)
target_link_libraries(rlpl PUBLIC libnajac)

add_executable(rppl rppl.cpp)
target_include_directories(rppl PUBLIC ${CMAKE_SOURCE_DIR}/libnajac)
target_link_libraries(rppl PUBLIC libnajac)

add_executable(rcpl rcpl.cpp)
target_include_directories(rcpl PUBLIC ${CMAKE_SOURCE_DIR}/libnajac)
target_link_libraries(rcpl PUBLIC libnajac)
