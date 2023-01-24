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
    source/syntax_analysis.cpp
    source/vardef_parsing.cpp
    source/fndef_parsing.cpp
    source/statement_parsing.cpp
    source/expression_parsing.cpp
)

target_precompile_headers(tyc
PRIVATE
  source/pch.hpp
)