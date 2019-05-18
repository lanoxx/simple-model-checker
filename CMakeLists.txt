cmake_minimum_required(VERSION 3.10)
project(model_checking C)

set(CMAKE_C_STANDARD 11)

find_package(PkgConfig)
find_package(BISON)

enable_testing()

pkg_check_modules (GLIB REQUIRED glib-2.0)
pkg_check_modules (JSONGLIB REQUIRED json-glib-1.0)

add_subdirectory(ctl-parser)
add_subdirectory(kripke)
add_subdirectory(model-checking)

add_executable(model_checking "")

target_sources(model_checking PRIVATE
        options.c
        options.h
        main.c)

target_include_directories(model_checking PRIVATE ${CMAKE_CURRENT_BINARY_DIR} ${GLIB_INCLUDE_DIRS} ctl-parser kripke model-checking)
target_link_libraries(model_checking ${GLIB_LIBRARIES} ctl-parser kripke model-checking)

set (BINARY_NAME model-checking)

configure_file(config.h.in config.h)
