cmake_minimum_required(VERSION 3.2)
project(MemoryAllocator)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.cpp MemoryManager.cpp MemoryManager.h)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${MemoryAllocator_SOURCE_DIR})
add_executable(MemoryAllocator ${SOURCE_FILES})


