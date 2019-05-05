find_package(BISON REQUIRED)
find_package(FLEX REQUIRED)

bison_target(parser calc.y ${CMAKE_BUILD_DIR}/calc.tab.c)
flex_target(lexer calc.lex ${CMAKE_BUILD_DIR}/lex.yy.c)

BISON_TARGET(parser ctl-parser.y ${CMAKE_CURRENT_BINARY_DIR}/bison.c)
FLEX_TARGET(lexer ctl.lex  ${CMAKE_CURRENT_BINARY_DIR}/lex.c)
ADD_FLEX_BISON_DEPENDENCY(lexer parser)

add_library(ctl-parser "")

target_sources(ctl-parser PRIVATE
        parser.c
        formula.c
        heading.h
        ${BISON_parser_OUTPUTS}
        ${FLEX_lexer_OUTPUTS})

target_include_directories(ctl-parser PRIVATE ${CMAKE_CURRENT_BINARY_DIR} ${CMAKE_CURRENT_SOURCE_DIR} ${GLIB_INCLUDE_DIRS})
target_link_libraries(ctl-parser ${GLIB_LIBRARIES} fl)
