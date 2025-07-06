
#include "mainwindow.h"
#include "ui_mainwindow.h"

// Use full Qt module paths
#include <QtCore/QTimer>
#include <QtCore/QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    initUI();
    startNewGame();
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::windowstart() {
    setWindowTitle("2048 AI Master");
    setFixedSize(500, 500);
    // Use standard 2048 background color
    this->setStyleSheet("background-color: #faf8ef;");
}

// Initializes UI components
void MainWindow::initUI() {
    // Window setup
    setWindowTitle("2048 AI Master");
    setFixedSize(500, 500);
    setStyleSheet("background-color: #faf8ef;"); // Classic 2048 background
    
    // Create central widget and layout
    centralWidget = new QWidget(this);
    gridLayout = new QGridLayout(centralWidget);
    gridLayout->setSpacing(10);
    setCentralWidget(centralWidget);
    
    // Create tile labels
    for(int i = 0; i < 4; i++) {
        for(int j = 0; j < 4; j++) {
            tiles[i][j] = new QLabel("", this);
            tiles[i][j]->setAlignment(Qt::AlignCenter);
            tiles[i][j]->setMinimumSize(100, 100);
            gridLayout->addWidget(tiles[i][j], i, j);
        }
    }
}

// Starts new game
void MainWindow::startNewGame() {
    gameBoard.begin();
    aiRunning = false;
    updateUI();
}

// Updates game display
void MainWindow::updateUI() {
    for(int i = 0; i < 4; i++) {
        for(int j = 0; j < 4; j++) {
            int value = gameBoard.brd[i][j];
            tiles[i][j]->setText(value ? QString::number(value) : "");
            updateTileAppearance(i, j);
        }
    }
    statusBar()->showMessage("Score: " + QString::number(gameBoard.score));
}

// Updates tile colors based on value
void MainWindow::updateTileAppearance(int i, int j) {
    int value = gameBoard.brd[i][j];
    QString style;
    
    switch(value) {
    case 0:    style = "background: #cdc1b4;"; break;
    case 2:    style = "background: #eee4da; color: #776e65;"; break;
    case 4:    style = "background: #ede0c8; color: #776e65;"; break;
    case 8:    style = "background: #f2b179; color: #f9f6f2;"; break;
    case 16:   style = "background: #f59563; color: #f9f6f2;"; break;
    case 32:   style = "background: #f67c5f; color: #f9f6f2;"; break;
    case 64:   style = "background: #f65e3b; color: #f9f6f2;"; break;
    case 128:  style = "background: #edcf72; color: #f9f6f2;"; break;
    case 256:  style = "background: #edcc61; color: #f9f6f2;"; break;
    case 512:  style = "background: #edc850; color: #f9f6f2;"; break;
    case 1024: style = "background: #edc53f; color: #f9f6f2;"; break;
    case 2048: style = "background: #edc22e; color: #f9f6f2;"; break;
    default:   style = "background: #3c3a32; color: #f9f6f2;"; break;
    }
    
    style += "font: bold 24pt; border-radius: 5px;";
    tiles[i][j]->setStyleSheet(style);
}

void MainWindow::looks(int i, int j) {
    int value = labels[i][j]->text().toInt();
    QString style;
    
    switch(value) {
    case 0:    style = "background: #cdc1b4;"; break;
    case 2:    style = "background: #eee4da; color: #776e65;"; break;
    case 4:    style = "background: #ede0c8; color: #776e65;"; break;
    case 8:    style = "background: #f2b179; color: #f9f6f2;"; break;
    case 16:   style = "background: #f59563; color: #f9f6f2;"; break;
    case 32:   style = "background: #f67c5f; color: #f9f6f2;"; break;
    case 64:   style = "background: #f65e3b; color: #f9f6f2;"; break;
    case 128:  style = "background: #edcf72; color: #f9f6f2;"; break;
    case 256:  style = "background: #edcc61; color: #f9f6f2;"; break;
    case 512:  style = "background: #edc850; color: #f9f6f2;"; break;
    case 1024: style = "background: #edc53f; color: #f9f6f2;"; break;
    case 2048: style = "background: #edc22e; color: #f9f6f2;"; break;
    default:   style = "background: #3c3a32; color: #f9f6f2;"; break;
    }
    
    style += " font: bold 24pt; border-radius: 5px;";
    labels[i][j]->setStyleSheet(style);
}

// Handles key presses
void MainWindow::keyPressEvent(QKeyEvent *event) {
    if(gameBoard.isFull()) return;
    
    switch(event->key()) {
    case Qt::Key_Up:    gameBoard.move('U'); break;
    case Qt::Key_Down:  gameBoard.move('D'); break;
    case Qt::Key_Left:  gameBoard.move('L'); break;
    case Qt::Key_Right: gameBoard.move('R'); break;
    case Qt::Key_Space: makeAIMove(); break;   // Single AI move
    case Qt::Key_A:     runAI(); break;        // Toggle autoplay
    case Qt::Key_N:     startNewGame(); break; // New game
    case Qt::Key_Z:     gameBoard.undo(); break; // Undo
    default: QMainWindow::keyPressEvent(event);
    }
    
    gameBoard.fillATile();
    updateUI();
    if(gameBoard.isFull()) gameOver();
}

// AI makes single move
void MainWindow::makeAIMove() {
    if(gameBoard.isFull()) return;
    
    // Execute AI move
    char bestMove = '$';
    int bestScore = -1000000;
    char moves[] = {'U','D','L','R'};
    
    for(char move : moves) {
        Board temp = gameBoard;
        temp.move(move);
        if(temp.brd != gameBoard.brd) { // Valid move
            int score = expectimax(temp, 3, false);
            if(score > bestScore) {
                bestScore = score;
                bestMove = move;
            }
        }
    }
    
    if(bestMove != '$') {
        gameBoard.move(bestMove);
        gameBoard.fillATile();
        updateUI();
    }
}

// Expectimax algorithm for AI
int MainWindow::expectimax(Board& board, int depth, bool isMax) {
    if(depth == 0 || board.isFull()) 
        return board.heuresticValue();
        
    if(isMax) { // AI's turn
        int bestScore = -1000000;
        char moves[] = {'U','D','L','R'};
        
        for(char move : moves) {
            Board temp = board;
            temp.move(move);
            if(temp.brd != board.brd) { // Valid move
                int score = expectimax(temp, depth-1, false);
                bestScore = max(bestScore, score);
            }
        }
        return bestScore;
    }
    else { // Random tile spawn simulation
        int totalScore = 0;
        int emptyCount = 0;
        
        for(int i = 0; i < board.SIZE; i++) {
            for(int j = 0; j < board.SIZE; j++) {
                if(board.brd[i][j] != 0) continue;
                
                emptyCount++;
                // Simulate 2 (90% probability)
                board.brd[i][j] = 2;
                totalScore += 0.9 * expectimax(board, depth-1, true);
                
                // Simulate 4 (10% probability)
                board.brd[i][j] = 4;
                totalScore += 0.1 * expectimax(board, depth-1, true);
                
                board.brd[i][j] = 0;
            }
        }
        
        return emptyCount ? totalScore / emptyCount : -10000;
    }
}

// Toggles AI autoplay
void MainWindow::runAI() {
    aiRunning = !aiRunning;
    if(aiRunning) {
        QTimer::singleShot(aiSpeed, this, [this]() {
            if(aiRunning && !gameBoard.isFull()) {
                makeAIMove();
                runAI(); // Continue sequence
            }
        });
    }
}

// Handles game over
void MainWindow::gameOver() {
    aiRunning = false;
    QMessageBox::information(this, "Game Over", 
        "Final Score: " + QString::number(gameBoard.score));
}