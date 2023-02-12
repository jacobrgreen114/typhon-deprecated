

add_executable(tyc
        source/main.cpp

        source/project_config.cpp
)

target_link_libraries(tyc
    PUBLIC
        typhon_lexer
        typhon_parser
        typhon_checker
        typhon_generator
)