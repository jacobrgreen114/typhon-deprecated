cmake_minimum_required(VERSION 3.20)

add_library(typhon_lexer
        src/token.cpp
        src/lexer.cpp
        src/lexer_sm.cpp
        src/lexer_comment.cpp
        src/lexer_number.cpp
        src/lexer_string.cpp
        src/lexer_identifier.cpp
        src/lexer_symbol.cpp
)

target_include_directories(typhon_lexer
    PUBLIC
        inc
)

target_precompile_headers(typhon_lexer
    PUBLIC
        "inc/token.hpp"
)

target_link_libraries(typhon_lexer
    PUBLIC
        typhon_core
)

target_msvc_runtime_library(typhon_lexer)