cmake_minimum_required(VERSION 3.20)

project(Typhon-Compiler)

set(CMAKE_C_STANDARD 23)
set(CMAKE_CXX_STANDARD 23)

option(OUTPUT_ASSEMBLY_ON_BUILD "Outputs generated assembly on compile")

if (${OUTPUT_ASSEMBLY_ON_BUILD})
    if (MSVC)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /FA")
    else()

    endif()
endif()


# Enable LTO
set(CMAKE_INTERPROCEDURAL_OPTIMIZATION ON)

add_executable(tyc
        source/main.cpp

        source/lexical_analysis/token.cpp source/lexical_analysis/token.hpp
        source/lexical_analysis/lexer.cpp source/lexical_analysis/lexer.hpp
        source/lexical_analysis/lexer_sm.cpp source/lexical_analysis/lexer_sm.hpp
        source/lexical_analysis/lexer_comment.cpp source/lexical_analysis/lexer_comment.hpp
        source/lexical_analysis/lexer_number.cpp source/lexical_analysis/lexer_number.hpp
        source/lexical_analysis/lexer_sm.cpp source/lexical_analysis/lexer_sm.hpp
        source/lexical_analysis/lexer_symbol.cpp source/lexical_analysis/lexer_symbol.hpp
        source/lexical_analysis/lexer_identifier.cpp source/lexical_analysis/lexer_identifier.hpp

        source/syntax_analysis/syntax_tree.cpp source/syntax_analysis/syntax_tree.hpp
        source/syntax_analysis/parser.cpp source/syntax_analysis/parser.hpp

        source/syntax_analysis/parser_def_var.cpp source/syntax_analysis/parser_def_var.hpp
        source/syntax_analysis/parser_def_func.cpp source/syntax_analysis/parser_def_func.hpp
        source/syntax_analysis/parser_def_struct.cpp source/syntax_analysis/parser_def_struct.hpp

        source/syntax_analysis/parser_statement.cpp source/syntax_analysis/parser_statement.hpp
        source/syntax_analysis/parser_expression.cpp source/syntax_analysis/parser_expression.hpp
        source/syntax_analysis/parser_statemachine.cpp source/syntax_analysis/parser_statemachine.hpp

        source/generation/generator.cpp source/generation/generator.hpp
        source/generation/gen_var.cpp source/generation/gen_var.hpp

        source/paths.cpp source/paths.hpp

        source/timer.hpp
        source/xml/serialization.cpp source/xml/serialization.hpp
        source/project_config.cpp source/project_config.hpp source/common.hpp source/generation/gen_common.cpp source/generation/gen_common.hpp source/generation/gen_expr.cpp source/generation/gen_expr.hpp source/generation/gen_stmt.cpp source/generation/gen_stmt.hpp source/generation/gen_func.cpp source/generation/gen_func.hpp source/generation/gen_struct.cpp source/generation/gen_struct.hpp source/lexical_analysis/lexer_string.cpp source/lexical_analysis/lexer_string.hpp source/syntax_analysis/parser_import.cpp source/syntax_analysis/parser_import.hpp)

target_precompile_headers(tyc
PRIVATE
    <array>
    <exception>
    <fstream>
    <map>
    <memory>
    <stack>
    <string>
    <string_view>
    <unordered_map>
    <utility>
    <vector>
    <filesystem>
    <iostream>
    <span>
    <chrono>
    <unordered_set>
    <set>
)

target_compile_definitions(tyc
PRIVATE
    # Uncomment to enable compilation tracing
    TRACE
    # Uncomment to make code generation human readable
    GENERATE_READABLE
)


install(
    TARGETS
        tyc
    CONFIGURATIONS
        Release
    RENAME
        tyc
    RUNTIME
        DESTINATION
            bin
)