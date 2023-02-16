
add_library(typhon_parser
    src/syntax_tree.cpp

    src/parser.cpp
    src/parser_sm.cpp

    src/parser_import.cpp

    src/parser_expression.cpp
    src/parser_stmt.cpp

    src/parser_def_var.cpp
    src/parser_def_func.cpp
    src/parser_def_struct.cpp
        src/parser_def_object.cpp src/parser_def_object.hpp src/parser_c.cpp src/parser_c.hpp)

target_include_directories(typhon_parser
    PUBLIC
        inc
)

target_precompile_headers(typhon_parser
    PUBLIC
        "inc/syntax_tree.hpp"
)

target_link_libraries(typhon_parser
    PUBLIC
        typhon_lexer
)

target_msvc_runtime_library(typhon_parser)