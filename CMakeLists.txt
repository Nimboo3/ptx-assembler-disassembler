cmake_minimum_required(VERSION 3.10)
project(mini_ptx_asm LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include_directories(${PROJECT_SOURCE_DIR}/include)

add_executable(asm src/main_asm.cpp src/assembler.cpp src/utils.cpp)
add_executable(disasm src/main_disasm.cpp src/disassembler.cpp src/utils.cpp)

# Add tests script into build tree for convenience (not compiled)
file(COPY tests DESTINATION ${CMAKE_BINARY_DIR})
file(COPY examples DESTINATION ${CMAKE_BINARY_DIR})
