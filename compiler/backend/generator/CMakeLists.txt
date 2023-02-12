

add_library(typhon_generator
        src/generator.cpp
        src/gen_common.cpp
        src/gen_expr.cpp
        src/gen_stmt.cpp
        src/gen_var.cpp
        src/gen_func.cpp
        src/gen_struct.cpp
)

target_include_directories(typhon_generator
    PUBLIC
        inc
)

target_link_libraries(typhon_generator
    PUBLIC
        typhon_core
)