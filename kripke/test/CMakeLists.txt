add_executable(kripke-test "")

target_sources(kripke-test PRIVATE kripke-test.c)

target_include_directories(kripke-test PRIVATE ${GLIB_INCLUDE_DIRS} ${JSONGLIB_INCLUDE_DIRS} kripke)
target_link_libraries(kripke-test ${GLIB_LIBRARIES} ${JSONGLIB_LIBRARIS} kripke)

add_test(kripke-test-suite kripke-test)

set_property(TEST kripke-test-suite APPEND PROPERTY ENVIRONMENT
        G_TEST_SRCDIR=${CMAKE_CURRENT_SOURCE_DIR})

set_property(TEST kripke-test-suite APPEND PROPERTY ENVIRONMENT
        G_TEST_BUILDDIR=${CMAKE_CURRENT_BINARY_DIR})
