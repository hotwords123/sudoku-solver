#include "SudokuSolver.h"
#include <iostream>
#include <algorithm>
#include <bitset>
#include <bit>

SudokuSolver::SudokuSolver(const Sudoku::Grid &grid) {
    dlx.build(4 * 9 * 9);

    // 0-80: cell constraints
    // 81-161: row constraints
    // 162-242: column constraints
    // 243-323: block constraints
    for (int i = 0; i < 9 * 9; i++) {
        int row = i / 9;
        int col = i % 9;
        int block = row / 3 * 3 + col / 3;

        auto add = [&](int value) {
            auto head = dlx.addNode(nullptr, i);
            dlx.addNode(head, 1 * 9 * 9 + row * 9 + value);
            dlx.addNode(head, 2 * 9 * 9 + col * 9 + value);
            dlx.addNode(head, 3 * 9 * 9 + block * 9 + value);
            decisions.emplace_back(i, value + 1);
        };

        int cell = grid.cells[i];
        if (cell == 0) {
            for (int j = 0; j < 9; j++) add(j);
        } else {
            add(cell - 1);
        }
    }
}

std::vector<Sudoku::Grid> SudokuSolver::solve(int limit) {
    this->limit = limit;
    search();
    return std::move(solutions);
}

bool SudokuSolver::search() {
    // find the column with the fewest 1s
    int min_col = -1, min_size = 10;
    for (auto node : dlx.columns()) {
        int col = node->columnIndex();
        int size = dlx.columnSize(col);
        if (size < min_size) {
            min_col = col;
            min_size = size;
        }
    }

    // if all columns are covered, we have a solution
    if (min_col == -1) {
        solutions.push_back(solution);
        return limit > 0 && (int)solutions.size() >= limit;
    }

    // otherwise, cover the column and try all possible rows
    dlx.cover(min_col);

    for (auto row : dlx.rows(min_col)) {
        // add the decision to the solution
        auto [pos, value] = decisions[row->rowIndex()];
        solution.cells[pos] = value;

        // cover all columns that are in the same row as the decision
        for (auto cell : dlx.cells(row)) {
            dlx.cover(cell->columnIndex());
        }

        // search for the next decision
        if (search()) return true;

        // backtrack and uncover the columns in reversed order
        for (auto cell : dlx.cells<DancingLink::kBackward>(row)) {
            dlx.uncover(cell->columnIndex());
        }

        solution.cells[pos] = 0;
    }

    // backtrack
    dlx.uncover(min_col);
    return false;
}
