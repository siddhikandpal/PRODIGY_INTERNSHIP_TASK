#include <iostream>
using namespace std;

const int GRID_SIZE = 9;

void printSudoku(int grid[GRID_SIZE][GRID_SIZE]);
bool solveSudoku(int grid[GRID_SIZE][GRID_SIZE]);
bool isValidMove(int grid[GRID_SIZE][GRID_SIZE], int row, int col, int num);
bool usedInRow(int grid[GRID_SIZE][GRID_SIZE], int row, int num);
bool usedInCol(int grid[GRID_SIZE][GRID_SIZE], int col, int num);
bool usedInBox(int grid[GRID_SIZE][GRID_SIZE], int boxStartRow, int boxStartCol, int num);
bool findEmptyCell(int grid[GRID_SIZE][GRID_SIZE], int& row, int& col);

int main() {
    int sudokuGrid[GRID_SIZE][GRID_SIZE] = {
        { 5, 3, 0, 0, 7, 0, 0, 0, 0 },
        { 6, 0, 0, 1, 9, 5, 0, 0, 0 },
        { 0, 9, 8, 0, 0, 0, 0, 6, 0 },
        { 8, 0, 0, 0, 6, 0, 0, 0, 3 },
        { 4, 0, 0, 8, 0, 3, 0, 0, 1 },
        { 7, 0, 0, 0, 2, 0, 0, 0, 6 },
        { 0, 6, 0, 0, 0, 0, 2, 8, 0 },
        { 0, 0, 0, 4, 1, 9, 0, 0, 5 },
        { 0, 0, 0, 0, 8, 0, 0, 7, 9 }
    };

    if (solveSudoku(sudokuGrid)) {
        cout << "Sudoku solved successfully:" << endl;
        printSudoku(sudokuGrid);
    } else {
        cout << "No solution exists for the given Sudoku puzzle." << endl;
    }

    return 0;
}

bool solveSudoku(int grid[GRID_SIZE][GRID_SIZE]) {
    int row, col;

    if (!findEmptyCell(grid, row, col)) {
        // No empty cell found, puzzle is solved
        return true;
    }

    for (int num = 1; num <= 9; num++) {
        if (isValidMove(grid, row, col, num)) {
            // Try placing the number
            grid[row][col] = num;

            // Recursively attempt to solve the rest of the puzzle
            if (solveSudoku(grid)) {
                return true;
            }

            // If placing the number didn't lead to a solution, backtrack
            grid[row][col] = 0;
        }
    }

    // No valid number found, need to backtrack
    return false;
}

bool findEmptyCell(int grid[GRID_SIZE][GRID_SIZE], int& row, int& col) {
    for (row = 0; row < GRID_SIZE; row++) {
        for (col = 0; col < GRID_SIZE; col++) {
            if (grid[row][col] == 0) {
                return true;
            }
        }
    }
    return false;
}

bool isValidMove(int grid[GRID_SIZE][GRID_SIZE], int row, int col, int num) {
    return !usedInRow(grid, row, num) &&
           !usedInCol(grid, col, num) &&
           !usedInBox(grid, row - row % 3, col - col % 3, num);
}

bool usedInRow(int grid[GRID_SIZE][GRID_SIZE], int row, int num) {
    for (int col = 0; col < GRID_SIZE; col++) {
        if (grid[row][col] == num) {
            return true;
        }
    }
    return false;
}

bool usedInCol(int grid[GRID_SIZE][GRID_SIZE], int col, int num) {
    for (int row = 0; row < GRID_SIZE; row++) {
        if (grid[row][col] == num) {
            return true;
        }
    }
    return false;
}

bool usedInBox(int grid[GRID_SIZE][GRID_SIZE], int boxStartRow, int boxStartCol, int num) {
    for (int row = 0; row < 3; row++) {
        for (int col = 0; col < 3; col++) {
            if (grid[row + boxStartRow][col + boxStartCol] == num) {
                return true;
            }
        }
    }
    return false;
}

void printSudoku(int grid[GRID_SIZE][GRID_SIZE]) {
    for (int row = 0; row < GRID_SIZE; row++) {
        for (int col = 0; col < GRID_SIZE; col++) {
            cout << grid[row][col] << " ";
        }
        cout << endl;
    }
}
