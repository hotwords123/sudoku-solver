#include "Sudoku.h"
#include "SudokuSolver.h"
#include <iostream>

int main(int argc, char **argv) {
    Sudoku sudoku;
    if (!sudoku.readFromFile("sudoku.txt")) {
        return 1;
    }

    SudokuSolver solver(sudoku.data());

    std::string param = "10"; // find 10 solutions by default
    if (argc == 2) {
        param = argv[1];
    }

    if (param == "alt") { // find solutions step by step in a human-like way
        std::cout << "Solving step by step...\n";

        bool filled = false;
        while (true) {
            if (solver.reduceOne()) {
                filled = true;
            } else {
                if (!filled) break;
                solver.solutionGrid().print();
                std::cout << "\n";
                filled = false;
                if (!solver.reduceGroup()) break;
            }
        }
    } else { // find solutions by depth-first search
        int limit = std::stoi(param);
        std::cout << "Solving by depth-first search (limit: " << limit << ")...\n";

        auto solutions = solver.solve(limit);
        if (solutions.empty()) {
            std::cout << "No solution found.\n";
        } else {
            std::cout << "Found " << solutions.size() << " solution(s).\n";
            for (const auto &solution : solutions) {
                solution.print();
                std::cout << "\n";
            }
        }
    }

    return 0;
}
