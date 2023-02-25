cmake_minimum_required(VERSION 3.20)

add_library(typhon_core
        src/paths.cpp
        src/source.cpp
        src/project_config.cpp

        src/xml/serialization.cpp
        inc/xml/rapid_xml.hpp)

target_include_directories(typhon_core
    PUBLIC
        inc
)

target_precompile_headers(typhon_core
    PUBLIC
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
        <future>
        <execution>
        <stdexcept>
        <cassert>
)

target_msvc_runtime_library(typhon_core)