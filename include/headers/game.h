#ifndef GAME_H
#define GAME_H

#include <vector>
#include <string>

const int BOARD_SIZE = 9;
const int NUM_UNCOVERED = 10;

class Game
{
public:
    Game();
    bool generateSudokuBoard();
    void coverSpaces();
    std::string serialize() const;
    void deserialize(const std::string &data);
    const std::vector<std::vector<int>> &getBoard() const;

private:
    std::vector<std::vector<int>> board;
    bool isSafe(int row, int col, int num) const;
    bool generateSudokuBoard(int row, int col);
};

#endif // GAME_H
