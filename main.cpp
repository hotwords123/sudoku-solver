#include "Sudoku.h"
#include <iostream>

int main() {
    Sudoku sudoku;
    if (!sudoku.readFromFile("sudoku.txt")) {
        return 1;
    }
    auto solutions = sudoku.solve(10);
    for (const auto &solution : solutions) {
        solution.print();
        std::cout << '\n';
    }
    return 0;
}
