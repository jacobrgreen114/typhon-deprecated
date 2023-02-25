

add_executable(tyc
        source/main.cpp
)

target_link_libraries(tyc
    PUBLIC
        typhon_lexer
        typhon_parser
        typhon_checker
        typhon_generator
)

target_msvc_runtime_library(tyc)

install(TARGETS tyc)