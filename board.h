#ifndef BOARD_H
#define BOARD_H
#include<vector>
using namespace std;

class Board
{
public:
    int SIZE;                    // Board dimensions (4x4)
    vector<vector<int>> brd;     // 2D game board
    int score;                   // Current player score

    Board();                     // Constructor initializes board
    void fillATile();            // Adds new tile (90% 2, 10% 4)
    void fillRandom();           // Fills board with random tiles (testing only)
    int getRandom2or4();         // Tile value generator
    int heuresticValue();        // Board evaluation for AI
    void begin();                // Starts new game
    bool valid(int, int);        // Checks position validity
    void move(char);             // Executes move direction
    void up();                   // Move up implementation
    void left();                 // Move left implementation
    void down();                 // Move down implementation
    void right();                // Move right implementation
    bool isFull();               // Checks if game is over
    void print();                // Prints board to console
    int score = 0;               // Initialize score
    void addScore(int value);    // Updates score
    void saveState();            // Saves state for undo
    void undo();                 // Reverts to previous state

private:
    vector<vector<vector<int>>> history; // Game state history
};

#endif // BOARD_H