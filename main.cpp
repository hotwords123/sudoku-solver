#include "Sudoku.h"
#include "SudokuSolver.h"
#include <iostream>

int main() {
    Sudoku sudoku;
    if (!sudoku.readFromFile("sudoku.txt")) {
        return 1;
    }

    SudokuSolver solver(sudoku.data());
    auto solutions = solver.solve(10);
    for (auto &solution : solutions) {
        solution.print();
        std::cout << '\n';
    }

    return 0;
}
