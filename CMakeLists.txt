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

set(TESTS_FOLDER "${CMAKE_CURRENT_SOURCE_DIR}/tests/Stanford")
file(GLOB_RECURSE TESTS_SOURCES "${TESTS_FOLDER}/*.c")
foreach(TEST_SOURCE_NAME IN LISTS TESTS_SOURCES)
        get_filename_component(TEST_MODULE_NAME "${TEST_SOURCE_NAME}" NAME_WE)
        set(TEST_CASE_NAME "Test-Case-${TEST_MODULE_NAME}")
        add_custom_command(TARGET BytecodeToDot POST_BUILD
                COMMAND "${LLVM_INSTALL_DIR}/bin/clang" -S -emit-llvm -o "${TEST_SOURCE_NAME}.ll" "${TEST_SOURCE_NAME}" -Xclang -disable-O0-optnone
                COMMAND "${LLVM_INSTALL_DIR}/bin/opt" -load-pass-plugin "${CMAKE_CURRENT_BINARY_DIR}/libBytecodeToDot.so" -passes=mem2reg,lcssa,BytecodeToDotPass -S "${TEST_SOURCE_NAME}.ll" -o "${TEST_SOURCE_NAME}.ll" 2>&1
                VERBATIM
        )
endforeach()
