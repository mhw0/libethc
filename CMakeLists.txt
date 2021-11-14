project(ethc)

cmake_minimum_required(VERSION 3.21.0)
find_package(PkgConfig REQUIRED)

add_executable(ethc src/main.c)
