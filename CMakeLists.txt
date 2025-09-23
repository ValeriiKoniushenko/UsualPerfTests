cmake_minimum_required(VERSION 3.30..)

project(UsualPerfTests VERSION 1.0 LANGUAGES CXX)

add_compile_options("-O3")
add_compile_options("-std=c++26")

add_subdirectory(googletest)
add_subdirectory(benchmark)

add_executable(TestMe main.cpp)
target_link_libraries(TestMe PUBLIC benchmark)
