cmake_minimum_required(VERSION 3.10)

# Project name
project(Asm2Mach C)

# Set C standard
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Set output directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# Include directories
include_directories(${CMAKE_SOURCE_DIR}/include)

# Source files
file(GLOB SOURCES ${CMAKE_SOURCE_DIR}/src/*.c)

# Create executable
add_executable(Asm2Mach ${SOURCES})
