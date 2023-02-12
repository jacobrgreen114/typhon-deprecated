cmake_minimum_required(VERSION 3.20)

project(Typhon)

function(target_msvc_runtime_library TARGET)
    if(MSVC)
        set_property(TARGET ${TARGET} PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
    endif()
endfunction()

add_subdirectory(compiler)