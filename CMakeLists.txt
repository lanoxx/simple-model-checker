cmake_minimum_required(VERSION 3.13)
project(model_checking C)

set(CMAKE_C_STANDARD 11)

find_package(PkgConfig)
find_package(BISON)

pkg_check_modules (GLIB REQUIRED glib-2.0)

add_subdirectory(ctl-parser)
add_subdirectory(kripke)
add_subdirectory(model-checking)

add_executable(model_checking "")

target_sources(model_checking PRIVATE main.c)

target_include_directories(model_checking PRIVATE ${GLIB_INCLUDE_DIRS} ctl-parser kripke model-checking)
target_link_libraries(model_checking ${GLIB_LIBRARIES} ctl-parser kripke model-checking)
