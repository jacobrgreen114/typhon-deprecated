
add_library(typhon_checker
        src/semantic_analyser.cpp
)

target_include_directories(typhon_checker
    PUBLIC
        inc
)

target_link_libraries(typhon_checker
    PUBLIC
        typhon_core
)

target_precompile_headers(typhon_checker
    PUBLIC
        "${CMAKE_CURRENT_SOURCE_DIR}/../../core/inc/token.hpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/../../core/inc/syntax_tree.hpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/../../core/inc/project_tree.hpp"
)