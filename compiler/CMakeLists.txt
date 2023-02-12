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

add_compile_definitions(
        # Uncomment to enable compilation tracing
        TRACE
        # Uncomment to make code generation human readable
        GENERATE_READABLE
)

add_subdirectory(core)
add_subdirectory(frontend)
add_subdirectory(backend)
add_subdirectory(tyc)

