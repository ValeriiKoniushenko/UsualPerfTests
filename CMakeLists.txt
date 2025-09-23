cmake_minimum_required(VERSION 3.30..)

project(UsualPerfTests VERSION 1.0 LANGUAGES CXX)

add_subdirectory(googletest)
add_subdirectory(benchmark)

add_executable(TestMe main.cpp)

