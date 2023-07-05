#pragma once

#include "Sudoku.h"
#include "DancingLink.h"

class SudokuSolver {
public:
    SudokuSolver(const Sudoku::Grid &grid);

    /**
     * Solves the Sudoku puzzle.
     * @param limit the maximum number of solutions to find
     * @return the solutions found
     */
    std::vector<Sudoku::Grid> solve(int limit = 1);

private:
    DancingLink dlx;
    std::vector<std::pair<int, int>> decisions;
    std::vector<Sudoku::Grid> solutions;
    Sudoku::Grid solution;
    int limit = 1;

    bool search();
};
