cmake_minimum_required(VERSION 3.20)
project(TinyEngine)

set(VCPKG_TARGET_TRIPLET x64-windows)
find_package(ZLIB REQUIRED)
find_package(PNG REQUIRED)


set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

enable_testing()
add_executable(TinyEngine main.cpp)
add_subdirectory(unittest)

target_link_libraries(TinyEngine PRIVATE ZLIB::ZLIB)
target_link_libraries(TinyEngine PRIVATE PNG::PNG)