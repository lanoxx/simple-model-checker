add_library(model-checking "")

target_sources(model-checking PRIVATE model-checker.c)

target_include_directories(model-checking
        PRIVATE
            ${GLIB_INCLUDE_DIRS} ctl-parser kripke
        PUBLIC
            $<INSTALL_INTERFACE:include>
            $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        )
target_link_libraries(model-checking PRIVATE ${GLIB_LIBRARIES} ctl-parser kripke)
