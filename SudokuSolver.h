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

    Sudoku::Grid &solutionGrid() { return solution; }

    /**
     * Returns whether the puzzle is solved.
     * @return true if the puzzle is solved, false otherwise
     */
    bool done() {
        auto columns = dlx.columns();
        return columns.begin() == columns.end();
    }

    /**
     * Reduces the puzzle by finding single columns.
     * @return true if the puzzle was reduced, false otherwise
     */
    bool reduceOne();

    /**
     * Reduces the puzzle by finding number groups.
     * @return true if the puzzle was reduced, false otherwise
     */
    bool reduceGroup();

private:
    DancingLink dlx;
    std::vector<std::pair<int, int>> decisions;
    std::vector<Sudoku::Grid> solutions;
    Sudoku::Grid solution;
    int limit = 1;

    /**
     * Depth-first search for solutions.
     * @return true if we have found enough solutions, false otherwise
     */
    bool search();
};
