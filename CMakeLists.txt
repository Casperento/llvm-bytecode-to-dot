cmake_minimum_required(VERSION 3.25)
project(llvm_bytecode_to_dot)

set(CMAKE_CXX_STANDARD 17)

file(GLOB_RECURSE SOURCES src/*.cpp)

add_executable(llvm_bytecode_to_dot ${SOURCES})
