
add_library(typhon_checker
        src/project_tree.cpp
        src/checker.cpp
        src/symbol_table.cpp
)

target_include_directories(typhon_checker
    PUBLIC
        inc
)

target_link_libraries(typhon_checker
    PUBLIC
        typhon_parser
)

target_precompile_headers(typhon_checker
    PUBLIC
        inc/project_tree.hpp
)

target_msvc_runtime_library(typhon_checker)
