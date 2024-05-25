cmake_minimum_required(VERSION 3.20)
project(llvm_bytecode_to_dot)

set(LLVM_INSTALL_DIR "" CACHE PATH "LLVM installation directory")
list(APPEND CMAKE_PREFIX_PATH "${LLVM_INSTALL_DIR}/lib/cmake/llvm/")

find_package(LLVM REQUIRED CONFIG)

include_directories(SYSTEM ${LLVM_INCLUDE_DIRS})
link_directories(${LINK_LIBRARY_DIRS})
add_definitions(${LLVM_DEFINITIONS})

set(CMAKE_CXX_STANDARD 17 CACHE STRING "")

if(NOT LLVM_ENABLE_RTTI)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-rtti")
endif()

file(GLOB_RECURSE SOURCES lib/*.cpp)
file(GLOB_RECURSE HEADERS include/*.h)

add_library(BytecodeToDot SHARED ${SOURCES} ${HEADERS})

