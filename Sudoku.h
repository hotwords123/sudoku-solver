#pragma once

#include <vector>
#include <string>

class Sudoku {
public:
    struct Grid {
        int cells[9 * 9]{};

        int operator()(int row, int col) const {
            return cells[row * 9 + col];
        }

        int &operator()(int row, int col) {
            return cells[row * 9 + col];
        }

        void print() const;
    };

    const Grid &data() const { return grid; }

    /**
     * Reads a Sudoku puzzle from the given file.
     * @param filename the name of the file to read from
     * @return true if the file is read successfully, false otherwise
     */
    bool readFromFile(const std::string &filename);

private:
    Grid grid;
};
