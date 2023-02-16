

add_executable(tyc
        source/main.cpp
        ../../libraries/core/src/cpp/builtins.hpp)

target_link_libraries(tyc
    PUBLIC
        typhon_lexer
        typhon_parser
        typhon_checker
        typhon_generator
)

target_msvc_runtime_library(tyc)

install(TARGETS tyc)