#include "game.h"
#include <cstdlib>
#include <ctime>
#include <sstream>

Game::Game() : board(BOARD_SIZE, std::vector<int>(BOARD_SIZE, 0))
{
    generateSudokuBoard();
    coverSpaces();
}

bool Game::isSafe(int row, int col, int num) const
{
    for (int i = 0; i < BOARD_SIZE; ++i)
    {
        if (board[row][i] == num || board[i][col] == num ||
            board[3 * (row / 3) + i / 3][3 * (col / 3) + i % 3] == num)
        {
            return false;
        }
    }
    return true;
}

bool Game::generateSudokuBoard(int row, int col)
{
    if (row == BOARD_SIZE)
        return true;

    int nextRow = (col == BOARD_SIZE - 1) ? row + 1 : row;
    int nextCol = (col + 1) % BOARD_SIZE;

    if (board[row][col] != 0)
    {
        return generateSudokuBoard(nextRow, nextCol);
    }

    for (int num = 1; num <= BOARD_SIZE; ++num)
    {
        if (isSafe(row, col, num))
        {
            board[row][col] = num;
            if (generateSudokuBoard(nextRow, nextCol))
                return true;
            board[row][col] = 0;
        }
    }
    return false;
}

bool Game::generateSudokuBoard()
{
    return generateSudokuBoard(0, 0);
}

void Game::coverSpaces()
{
    std::srand(std::time(nullptr));
    int count = 0;
    while (count < BOARD_SIZE * BOARD_SIZE - NUM_UNCOVERED)
    {
        int row = std::rand() % BOARD_SIZE;
        int col = std::rand() % BOARD_SIZE;
        if (board[row][col] != 0)
        {
            board[row][col] = 0;
            ++count;
        }
    }
}

std::string Game::serialize() const
{
    std::ostringstream oss;
    for (const auto &row : board)
    {
        for (int cell : row)
        {
            oss << cell << ' ';
        }
    }
    return oss.str();
}

void Game::deserialize(const std::string &data)
{
    std::istringstream iss(data);
    for (auto &row : board)
    {
        for (int &cell : row)
        {
            iss >> cell;
        }
    }
}

const std::vector<std::vector<int>> &Game::getBoard() const
{
    return board;
}
