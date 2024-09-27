cmake_minimum_required(VERSION 3.1)
project(tinyobjloader)

add_library(tinyobjloader)
target_sources(tinyobjloader PRIVATE "src/tiny_obj_loader.cpp")
target_include_directories(tinyobjloader PUBLIC include)
