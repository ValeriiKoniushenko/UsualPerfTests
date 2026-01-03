cmake_minimum_required(VERSION 3.30..)

project(UsualPerfTests VERSION 1.0 LANGUAGES CXX)


add_subdirectory(googletest)
add_subdirectory(benchmark)

add_compile_options("-std=c++23")

add_library(Setup INTERFACE)
target_compile_definitions(Setup INTERFACE PATH_TO_TEMPLATE_HTML="${CMAKE_CURRENT_SOURCE_DIR}/template.html" )
target_compile_options(Setup INTERFACE "-O3")

add_executable(Daily Daily.cpp)
target_link_libraries(Daily PUBLIC benchmark Setup)

add_executable(Misc Misc.cpp)
target_link_libraries(Misc PUBLIC benchmark Setup)

add_executable(Findings Findings.cpp)
target_link_libraries(Findings PUBLIC benchmark Setup)

add_executable(Pointers Pointers.cpp)
target_link_libraries(Pointers PUBLIC benchmark Setup)
