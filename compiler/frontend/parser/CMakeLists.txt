
add_library(typhon_parser
    src/parser.cpp
    src/parser_statemachine.cpp

    src/parser_import.cpp

    src/parser_expression.cpp
    src/parser_statement.cpp

    src/parser_def_var.cpp
    src/parser_def_func.cpp
    src/parser_def_struct.cpp
)

target_include_directories(typhon_parser
    PUBLIC
        inc
)

target_precompile_headers(typhon_parser
    PUBLIC
        "${CMAKE_CURRENT_SOURCE_DIR}/../../core/inc/token.hpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/../../core/inc/syntax_tree.hpp"
)

target_link_libraries(typhon_parser
    PUBLIC
        typhon_core
)