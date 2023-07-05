cmake_minimum_required(VERSION 3.9)
project(sudoku)

# Set C++ standard
set(CMAKE_CXX_STANDARD 20)

# Add source files
set(SOURCES
    main.cpp
    Sudoku.cpp
    SudokuSolver.cpp
    DancingLink.cpp
)

# Add header files
set(HEADERS
    Sudoku.h
    SudokuSolver.h
    DancingLink.h
)

if(CMAKE_COMPILER_IS_GNUCXX)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")
endif()

# Add executable
add_executable(sudoku ${SOURCES} ${HEADERS})
