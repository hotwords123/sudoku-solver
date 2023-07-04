cmake_minimum_required(VERSION 3.9)
project(sudoku)

# Set C++ standard
set(CMAKE_CXX_STANDARD 20)

# Add source files
set(SOURCES
    main.cpp
    Sudoku.cpp
    DancingLink.cpp
)

# Add header files
set(HEADERS
    Sudoku.h
    DancingLink.h
)

# Add executable
add_executable(sudoku ${SOURCES} ${HEADERS})
