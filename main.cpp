#include "Sudoku.h"
#include "SudokuSolver.h"
#include <iostream>

int main() {
    Sudoku sudoku;
    if (!sudoku.readFromFile("sudoku.txt")) {
        return 1;
    }

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

    return 0;
}
