cmake_minimum_required(VERSION 3.10)
project(utasort)
set(CMAKE_CXX_STANDARD 17)
find_package(ZLIB)
find_package(TagLib REQUIRED)
add_executable(utasort 
    src/cli_main.cpp
    src/get_files.cpp
    src/PathBuilder.cpp
)
target_link_libraries(utasort 
    TagLib::tag
)
