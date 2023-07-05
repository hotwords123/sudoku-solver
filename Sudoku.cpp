#include "Sudoku.h"
#include "SudokuSolver.h"
#include <iostream>
#include <fstream>

void Sudoku::Grid::print() const {
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            std::cout << cells[i * 9 + j] << ' ';
            if (j == 2 || j == 5) std::cout << "| ";
        }
        std::cout << '\n';

        if (i == 2 || i == 5) {
            std::cout << "------+-------+------\n";
        }
    }
}

bool Sudoku::readFromFile(const std::string &filename) {
    std::ifstream fin(filename);
    if (!fin) {
        std::cerr << "error: failed to open " << filename << std::endl;
        return false;
    }

    for (int i = 0; i < 81; i++) {
        int &cell = grid.cells[i];
        if (!(fin >> cell)) {
            std::cerr << "error: failed to read cell " << i << std::endl;
            return false;
        }

        if (cell < 0 || cell > 9) {
            std::cerr << "error: cell " << i << " has invalid value " << cell << std::endl;
            return false;
        }
    }

    return true;
}
