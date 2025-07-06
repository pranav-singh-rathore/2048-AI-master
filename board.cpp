#include "board.h"
#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <utility>
using namespace std;

#define vi vector<int>
#define pb push_back

// Constructor: Initializes new game
Board::Board() {
    score = 0;
    begin();
}

// Starts new game with two random tiles
void Board::begin() {
    SIZE = 4;
    brd.clear();
    brd.resize(SIZE, vi(SIZE, 0));
    int i = rand() % (SIZE * SIZE), ni = i;
    brd[i / SIZE][i % SIZE] = getRandom2or4();
    do { ni = rand() % (SIZE * SIZE); } while (ni == i);
    brd[ni / SIZE][ni % SIZE] = getRandom2or4();
    history.clear(); // Clear game history
}

// Checks if position is within board bounds
bool Board::valid(int x, int y) {
    return (x >= 0 && x < SIZE && y >= 0 && y < SIZE);
}

// Adds value to player's score
void Board::addScore(int value) {
    score += value;
}

// Saves current state to history
void Board::saveState() {
    history.push_back(brd);
    // Keep only last 10 states
    if(history.size() > 10) history.erase(history.begin());
}

// Reverts to previous state
void Board::undo() {
    if(!history.empty()) {
        brd = history.back();
        history.pop_back();
    }
}

// Prints board to console (debugging)
void Board::print() {
    for(auto& x: brd) {
        for(auto& y: x) {
            cout << y << " ";
        }
        cout << "\n";
    }
}

// Adds new tile to random empty position
void Board::fillATile() {
    int emptyCount = 0;
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            emptyCount += (brd[i][j] == 0);
        }
    }
    if(emptyCount == 0) return;
    
    int target = rand() % emptyCount;
    int current = 0;
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            if (brd[i][j] == 0) {
                if (current == target) {
                    brd[i][j] = getRandom2or4();
                    return;
                }
                current++;
            }
        }
    }
}

// Heuristic evaluation: Prioritizes high-value tiles in corners
int Board::heuresticValue() {
    // Weighted position matrix (snake pattern)
    vector<vector<int>> weights = {
        {12, 13, 14, 15},  // Top row (most valuable)
        {11, 10, 9, 8},     // Second row
        {4, 5, 6, 7},       // Third row
        {3, 2, 1, 0}        // Bottom row (least valuable)
    };
    
    int value = 0;
    for(int i = 0; i < SIZE; i++) {
        for(int j = 0; j < SIZE; j++) {
            value += (weights[i][j] * brd[i][j]);
        }
    }
    return value;
}

// Checks if game is over (no moves left)
bool Board::isFull() {
    // Check for empty spaces
    for(int i = 0; i < SIZE; i++)
        for(int j = 0; j < SIZE; j++)
            if(brd[i][j] == 0) return false;

    // Check for possible merges
    const int dx[] = {0, 0, 1, -1};
    const int dy[] = {1, -1, 0, 0};
    for (int i = 0; i < SIZE; i++) {
        for(int j = 0; j < SIZE; j++) {
            for(int k = 0; k < 4; k++) {
                int ni = i + dx[k], nj = j + dy[k];
                if(valid(ni, nj) && brd[ni][nj] == brd[i][j]) 
                    return false;
            }
        }
    }
    return true;
}

// Returns 2 (90%) or 4 (10%)
int Board::getRandom2or4() {
    return (rand() % 100 < 90) ? 2 : 4;
}

// Move dispatcher
void Board::move(char c) {
    saveState(); // Save before move
    switch(c) {
        case 'U': up(); break;
        case 'L': left(); break;
        case 'R': right(); break;
        case 'D': down(); break;
    }
}

// Move up implementation
void Board::up() {
    for (int j = 0; j < SIZE; j++) {
        // Compress non-zero tiles
        vi nonZero;
        for (int i = 0; i < SIZE; i++) {
            if (brd[i][j] != 0) nonZero.pb(brd[i][j]);
        }
        for (int i = 0; i < nonZero.size(); i++) {
            brd[i][j] = nonZero[i];
        }
        for (int i = nonZero.size(); i < SIZE; i++) {
            brd[i][j] = 0;
        }
        
        // Merge tiles
        for (int i = 0; i < SIZE-1; i++) {
            if (brd[i][j] != 0 && brd[i][j] == brd[i+1][j]) {
                brd[i][j] *= 2;
                addScore(brd[i][j]); // Add to score
                brd[i+1][j] = 0;
            }
        }
        
        // Recompress after merge
        nonZero.clear();
        for (int i = 0; i < SIZE; i++) {
            if (brd[i][j] != 0) nonZero.pb(brd[i][j]);
        }
        for (int i = 0; i < nonZero.size(); i++) {
            brd[i][j] = nonZero[i];
        }
        for (int i = nonZero.size(); i < SIZE; i++) {
            brd[i][j] = 0;
        }
    }
}


void Board::down(){
    for (int i = 0; i < brd.size() / 2; i++) {
        for (int j = 0; j < brd.size(); j++) {
            swap(brd[i][j], brd[SIZE - 1 - i][j]);
        }
    }

    up();

    for (int i = 0; i < brd.size() / 2; i++) {
        for (int j = 0; j < brd.size(); j++) {
            swap(brd[i][j], brd[SIZE - 1 - i][j]);
        }
    }
}

void Board::left(){
    for (int i = 0; i < SIZE; i++) {
        vi nZ;
        for (int j = 0; j < SIZE; j++) {if (brd[i][j] != 0)	nZ.pb(brd[i][j]);}
        for (int j = 0; j < nZ.size(); j++) {brd[i][j] = nZ[j];}
        for (int j = nZ.size(); j < SIZE; j++) {brd[i][j] = 0;}
    }
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            if (valid(i, j + 1) && brd[i][j] != 0 && brd[i][j] == brd[i][j + 1]) {
                brd[i][j] *= 2;
                brd[i][j + 1] = 0;
            }
        }
    }
    for (int i = 0; i < SIZE; i++) {
        vi nZ;
        for (int j = 0; j < SIZE; j++) {if (brd[i][j] != 0)	nZ.pb(brd[i][j]);}
        for (int j = 0; j < nZ.size(); j++) {brd[i][j] = nZ[j];}
        for (int j = nZ.size(); j < SIZE; j++) {brd[i][j] = 0;}
    }
}

void Board::right(){
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE / 2; j++) {
            swap(brd[i][j], brd[i][SIZE - 1 - j]);
        }
    }
    left();
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE / 2; j++) {
            swap(brd[i][j], brd[i][SIZE - 1 - j]);
        }
    }
}
