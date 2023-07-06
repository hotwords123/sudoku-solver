# sudoku-solver

This program allows you to recognize and solve Sudoku puzzles using OCR and a Sudoku solver written in C++.

## Prerequisites

Before using this program, ensure that you have the following dependencies installed:

- Python (version 3 or above)
- OpenCV Python library (for image processing)
- Tesseract OCR engine (for text recognition)
- CMake (for building the C++ program)
- C++ compiler (such as g++)

Then install the required Python packages by running the following command:
```sh
pip install -r requirements.txt
```

## Usage

1. Place the screenshot of the Sudoku puzzle in a file named `screenshot.png`.
   Ensure that the puzzle is clear and visible in the image.

2. Run the OCR program by executing the following command:
   ```sh
   python scan.py
   ```
   The OCR program will process the image, recognize the numbers, and save the OCR result in a file named `sudoku.txt`.
   Alternatively, you can put the puzzle directly into `sudoku.txt`. The file `sudoku-example.txt` shows what a typical puzzle would look like.

3. Build the C++ Sudoku solver program using CMake.
   In the project directory, execute the following commands:
   ```sh
   cmake .
   cmake --build .
   ```
   This will generate the necessary build files and compile the C++ program.

4. Run the Sudoku solver program by executing the generated executable. Use one of the following commands:
   ```sh
   ./sudoku 10  # find 10 solutions (same as `./sudoku`)
   ./sudoku alt # find the solution step by step (might fail for some puzzles)
   ```
   The program will read the Sudoku puzzle from the `sudoku.txt` file, solve it, and display the solutions.

## Additional Notes

- Ensure that the Sudoku puzzle in the screenshot is clear and well-aligned. Blurry or distorted images may affect the OCR accuracy.

- If the OCR results are not accurate, you can experiment with different image preprocessing techniques, such as adjusting brightness/contrast, applying filters, or resizing the image.

- Make sure that the Sudoku puzzle is a standard 9x9 grid with numbers from 1 to 9. The OCR program and Sudoku solver are designed to work with standard Sudoku puzzles.

- Feel free to modify and extend the OCR program and Sudoku solver to suit your specific needs or add additional features.

- This README is mainly written by ChatGPT.

Happy Sudoku solving!
