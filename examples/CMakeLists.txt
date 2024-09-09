cmake_minimum_required(VERSION 3.10)
project(template C CXX ASM)

include(GNUInstallDirs)
set(CMAKE_BUILD_TYPE Release)

subdirs(
basicobjects
celshading
convexobjects
heightmaps
lights
mode2d
mp3player
multitexturing
oslib
platform2d
reflection
rendertotexture
template
utility
amgpp_test
)
