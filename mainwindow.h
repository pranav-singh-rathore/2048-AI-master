#ifndef MAINWINDOW_H
#define MAINWINDOW_H

// Use full Qt module paths
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QLabel>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QMessageBox>
#include <QtGui/QKeyEvent>
#include "board.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    // Constructor and destructor
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    // Event handlers
    virtual void keyPressEvent(QKeyEvent *event) override;

private:
    // Game functions
    void initUI();                 // Initializes user interface
    void updateUI();               // Updates game display
    void startNewGame();           // Starts new game
    void gameOver();               // Handles game end
    
    // AI functions
    void makeAIMove();             // AI makes single move
    void runAI();                  // Continuous AI play
    
    // Helper functions
    void updateTileAppearance(int i, int j); // Updates tile colors
    int expectimax(Board& board, int depth, bool isMax); // AI algorithm
    
    // UI components
    Ui::MainWindow *ui;
    QWidget *centralWidget;
    QGridLayout *gridLayout;
    QLabel* tiles[4][4];           // Game tiles
    
    // Game state
    Board gameBoard;               // Game logic
    bool aiRunning = false;        // AI autoplay state
    int aiSpeed = 300;             // AI move delay (ms)
};

#endif // MAINWINDOW_H