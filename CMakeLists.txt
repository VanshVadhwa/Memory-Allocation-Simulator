cmake_minimum_required(VERSION 3.10)
project(MemoryAllocator)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add the main executable
add_executable(memory_allocator os.cpp)

# Optional: Add compiler flags for better warnings
if(MSVC)
    target_compile_options(memory_allocator PRIVATE /W4)
else()
    target_compile_options(memory_allocator PRIVATE -Wall -Wextra -Wpedantic)
endif()

# Installation configuration
install(TARGETS memory_allocator DESTINATION bin)
