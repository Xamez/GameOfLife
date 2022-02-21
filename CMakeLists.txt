cmake_minimum_required(VERSION 3.15)

project(GameOfLife CXX)

add_executable(GameOfLife main.cpp olcPixelGameEngine.h)

include(cmake/PGE.cmake)
pge_set_emscripten_options(main)
