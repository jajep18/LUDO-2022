cmake_minimum_required(VERSION 3.1)
set (CMAKE_CXX_STANDARD 11)
project( Domain-Code )

file(GLOB SOURCES
    Domain-Code/*.h
    Domain-Code/*.hpp
    Domain-Code/*.cpp
)

add_executable( main ${SOURCES} )