cmake_minimum_required(VERSION 3.30..)

project(UsualPerfTests VERSION 1.0 LANGUAGES CXX)


add_subdirectory(googletest)
add_subdirectory(benchmark)

add_compile_options("-std=c++23")

add_library(Exporter INTERFACE)
target_compile_definitions(Exporter INTERFACE PATH_TO_TEMPLATE_HTML="${CMAKE_CURRENT_SOURCE_DIR}/template.html" )

add_executable(Daily Daily.cpp)
target_link_libraries(Daily PUBLIC benchmark Exporter)

add_executable(Misc Misc.cpp)
target_link_libraries(Misc PUBLIC benchmark Exporter)

add_executable(Findings Findings.cpp)
target_link_libraries(Findings PUBLIC benchmark Exporter)

add_executable(Pointers Pointers.cpp)
target_link_libraries(Pointers PUBLIC benchmark Exporter)
