cmake_minimum_required(VERSION 3.10)
project(FTL4)
# set(SFML_DIR "/path/to/SFML/lib/cmake/SFML")
find_package(SFML 2.5.1 REQUIRED COMPONENTS graphics window system)
add_executable(FTL4
    src/main.cpp
    src/config.h
    src/button.h
    src/editor.h
    src/algo.h
    src/layout.h
    src/dialog.h
    src/callbacks.h
)
include_directories("src")
target_link_libraries(FTL4 PRIVATE sfml-graphics)
target_compile_features(FTL4 PRIVATE cxx_std_17)
