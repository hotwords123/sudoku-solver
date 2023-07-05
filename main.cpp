#include "Sudoku.h"
#include "SudokuSolver.h"
#include <iostream>

int main() {
    Sudoku sudoku;
    if (!sudoku.readFromFile("sudoku.txt")) {
        return 1;
    }

    // find first 10 solutions of the puzzle
    auto solve = [&]() {
    SudokuSolver solver(sudoku.data());
        auto solutions = solver.solve(10);
        for (const auto &solution : solutions) {
            solution.print();
            std::cout << "\n";
        }
    };

    // find solutions in a human-like way
    auto test = [&]() {
        SudokuSolver solver(sudoku.data());
        bool ok = true;
        while (ok) {
            ok = false;
            while (solver.reduceOne()) {
                ok = true;
            }
            if (solver.reduceGroup()) {
                ok = true;
            }
            solver.solutionGrid().print();
            std::cout << "\n";
        }
    };

    if (true) {
        solve();
    } else {
        test();
    }

    return 0;
}
