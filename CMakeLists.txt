cmake_minimum_required(VERSION 3.30..)

project(UsualPerfTests VERSION 1.0 LANGUAGES CXX)


add_subdirectory(googletest)
add_subdirectory(benchmark)

add_compile_options("-std=c++23")
add_executable(TestMe main.cpp)
target_link_libraries(TestMe PUBLIC benchmark)

target_compile_definitions(TestMe PRIVATE PATH_TO_TEMPLATE_HTML="${CMAKE_CURRENT_SOURCE_DIR}/template.html" )