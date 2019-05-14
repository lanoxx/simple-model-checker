add_library(kripke "")

target_sources(kripke PRIVATE kripke.c)

target_include_directories(kripke
        PRIVATE
            ${GLIB_INCLUDE_DIRS}
            ${JSONGLIB_INCLUDE_DIRS}
        PUBLIC
            $<INSTALL_INTERFACE:include>
            $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        )
target_link_libraries(kripke PRIVATE ${GLIB_LIBRARIES} ${JSONGLIB_LIBRARIES})

add_subdirectory(test)
