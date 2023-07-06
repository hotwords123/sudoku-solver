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
            dlx.addNode(i, true);
            dlx.addNode(1 * 9 * 9 + row * 9 + value);
            dlx.addNode(2 * 9 * 9 + col * 9 + value);
            dlx.addNode(3 * 9 * 9 + block * 9 + value);
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

    bool done = false;

    for (auto row : dlx.rows(min_col)) {
        // add the decision to the solution
        auto [pos, value] = decisions[row->rowIndex()];
        solution.cells[pos] = value;

        // cover all columns that are in the same row as the decision
        for (auto cell : dlx.cells(row)) {
            dlx.cover(cell->columnIndex());
        }

        // search for the next decision
        done = search();

        // backtrack and uncover the columns in reversed order
        for (auto cell : dlx.cells<DancingLink::kBackward>(row)) {
            dlx.uncover(cell->columnIndex());
        }

        solution.cells[pos] = 0;

        if (done) break;
    }

    // backtrack
    dlx.uncover(min_col);
    return done;
}

static std::string formatCell(int pos) {
    int row = pos / 9 + 1;
    int col = pos % 9 + 1;
    return "(" + std::to_string(row) + ", " + std::to_string(col) + ")";
}

bool SudokuSolver::reduceOne() {
    for (auto col : dlx.columns()) {
        int col_index = col->columnIndex();

        // if a column has only one row, we can make a decision
        if (dlx.columnSize(col_index) == 1) {
            dlx.cover(col_index);

            // add the decision to the solution
            auto row = *dlx.rows(col_index).begin();
            auto [pos, value] = decisions[row->rowIndex()];
            solution.cells[pos] = value;
            std::cout << "reduceOne: cell " << formatCell(pos) << " is " << value << "\n";

            for (auto cell : dlx.cells(row)) {
                dlx.cover(cell->columnIndex());
            }

            return true;
        }
    }

    return false;
}

bool SudokuSolver::reduceGroup() {
    using uint = unsigned int;

    auto columns = dlx.columns();
    auto iter = columns.begin();
    // skip all cell constraints
    while (iter != columns.end() && iter->columnIndex() < 9 * 9) ++iter;

    std::vector<int> cells; // the cells in the constraint
    std::vector<uint> masks(9); // the masks of the cells

    /**
     * Returns the index of a cell in the constraint.
     * @param col_index the column index of the cell
     * @return the index of the cell
     */
    auto getIndex = [&](int col_index) -> int {
        auto iter = std::find(cells.begin(), cells.end(), col_index);
        int index = iter - cells.begin();
        if (iter == cells.end()) cells.push_back(col_index);
        return index;
    };

    std::vector<int> group; // the rows in the group

    std::vector<DancingLink::node_ptr> unused_rows;
    bool ok = false; // whether at least one row is removed

    /**
     * Removes all unused rows.
     * Sets `ok` to true if at least one row is removed.
     */
    auto removeRows = [&]() {
        if (unused_rows.empty()) return;

        for (auto row : unused_rows) {
            auto [pos, value] = decisions[row->rowIndex()];
            std::cout << " * cell " << formatCell(pos) << " is not " << value << "\n";
            dlx.removeRow(row);
        }

        ok = true;
    };

    for (int i = 9; i < 4 * 9; i++) {
        cells.clear();
        std::fill(masks.begin(), masks.end(), 0);

        uint number_mask = 0; // numbers that appear in the constraint

        // enumerate all columns in the constraint
        for (; iter != columns.end() && iter->columnIndex() < 9 * (i + 1); ++iter) {
            int number = iter->columnIndex() % 9;
            number_mask |= 1u << number;

            // find all related cells and put them into the mask
            for (auto row : dlx.rows(iter->columnIndex())) {
                auto [pos, value] = decisions[row->rowIndex()];
                masks[number] |= 1u << getIndex(pos);
            }
        }

        // Reduction 1.
        // If a group of numbers appears only in a equal number of cells, then
        // these cells cannot contain any other numbers.

        // enumerate all subsets of the constraint
        for (uint set = number_mask; set != 0; --set &= number_mask) {
            // find all covered cells
            uint covered = 0;
            for (int number = 0; number < 9; number++) {
                if (set >> number & 1u) {
                    covered |= masks[number];
                }
            }

            int set_size = std::popcount(set);
            int covered_count = std::popcount(covered);
            // if #covered == #subset, we can group the rows
            if (set_size != covered_count) continue;

            std::cout << "reduceGroup: found a group of " << set_size << " cells\n";

            unused_rows.clear();
            for (int index = 0; index < (int)cells.size(); index++) {
                if (covered >> index & 1u) {
                    for (auto row : dlx.rows(cells[index])) {
                        auto [pos, value] = decisions[row->rowIndex()];
                        if (!(set >> (value - 1) & 1u)) {
                            // if the row is not in the group, we can remove it
                            unused_rows.push_back(row);
                        }
                    }
                }
            }
            removeRows();
        }

        // Reduction 2.
        // If a number appears only in a group of cells, then these cells
        // must contain the number.

        for (int number = 0; number < 9; number++) {
            if (number_mask >> number & 1u) {
                std::cout << " - number " << (number + 1) << ":";

                // find all rows that contain the number
                group.clear();
                for (auto row : dlx.rows(9 * i + number)) {
                    auto [pos, value] = decisions[row->rowIndex()];
                    std::cout << " " << formatCell(pos);
                    group.push_back(row->rowIndex());
                }
                std::cout << "\n";

                // enumerate all columns
                // if a column contains all rows in the group, we can remove all other rows
                for (auto col : columns) [&]() {
                    auto rows = dlx.rows(col->columnIndex());
                    auto iter = rows.begin();

                    unused_rows.clear();
                    for (int row_index : group) {
                        while (iter != rows.end() && iter->rowIndex() < row_index) {
                            unused_rows.push_back(*iter);
                            ++iter;
                        }
                        if (iter == rows.end() || iter->rowIndex() != row_index)
                            return; // the column does not contain all rows in the group
                        ++iter;
                    }
                    while (iter != rows.end()) {
                        unused_rows.push_back(*iter);
                        ++iter;
                    }
                    removeRows();
                }();
            }
        }
    }

    return ok;
}
