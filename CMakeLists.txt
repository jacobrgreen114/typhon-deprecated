cmake_minimum_required(VERSION 3.20)

project(Typhon)

set(CMAKE_C_STANDARD 23)
set(CMAKE_CXX_STANDARD 23)

# Enable LTO
set(CMAKE_INTERPROCEDURAL_OPTIMIZATION TRUE)

add_executable(tyc
    source/main.cpp
    source/lexical_analysis/token.cpp
    source/lexical_analysis/lexer.cpp
    source/lexical_analysis/lexer_source.cpp
    source/lexical_analysis/lexer_comment.cpp
    source/lexical_analysis/lexer_number.cpp
    source/lexical_analysis/lexer_source.cpp
    source/lexical_analysis/lexer_symbol.cpp
    source/lexical_analysis/lexer_identifier.cpp
    source/syntax_analysis/syntax.cpp
    source/syntax_analysis/parser.cpp
    source/syntax_analysis/parser_def_var.cpp
    source/syntax_analysis/parser_def_func.cpp
    source/syntax_analysis/parser_statement.cpp
    source/syntax_analysis/parser_expression.cpp
    source/syntax_analysis/parser_statemachine.cpp
)

target_precompile_headers(tyc
PRIVATE
  source/pch.hpp
)

target_compile_definitions(tyc
PRIVATE
    TRACE
)