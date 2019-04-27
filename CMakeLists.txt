cmake_minimum_required(VERSION 3.13)
project(model_checking C)

set(CMAKE_C_STANDARD 11)

find_package(PkgConfig)

pkg_check_modules (GLIB REQUIRED glib-2.0)

add_executable(model_checking "")

target_sources(model_checking PRIVATE main.c)

target_include_directories(model_checking PRIVATE ${GLIB_INCLUDE_DIRS})
target_link_libraries(model_checking ${GLIB_LIBRARIES})
