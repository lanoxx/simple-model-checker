add_executable(ctl-parser-test "")

target_sources(ctl-parser-test PRIVATE parser-test.c)

target_include_directories(ctl-parser-test PRIVATE ${GLIB_INCLUDE_DIRS} ctl-parser)
target_link_libraries(ctl-parser-test ${GLIB_LIBRARIES} ctl-parser)

add_test(ctl-parser-test-suite ctl-parser-test)

set_property(TEST ctl-parser-test-suite APPEND PROPERTY ENVIRONMENT
        G_TEST_SRCDIR=${CMAKE_CURRENT_SOURCE_DIR})

set_property(TEST ctl-parser-test-suite APPEND PROPERTY ENVIRONMENT
        G_TEST_BUILDDIR=${CMAKE_CURRENT_BINARY_DIR})
