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

    /**
     * Reads a Sudoku puzzle from the given file.
     * @param filename the name of the file to read from
     * @return true if the file is read successfully, false otherwise
     */
    bool readFromFile(const std::string &filename);

    /**
     * Solves the Sudoku puzzle.
     * @param limit the maximum number of solutions to find
     * @return the solutions found
     */
    std::vector<Grid> solve(int limit = 1);

private:
    Grid grid;
};
