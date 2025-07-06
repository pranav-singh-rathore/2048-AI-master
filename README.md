# 2048 AI Master

![Game Screenshot](screenshot.png)

A Qt implementation of the popular 2048 game with AI capabilities.

## Features
- Classic 2048 gameplay with smooth animations
- AI opponent using Expectimax algorithm
- Score tracking
- Undo functionality
- Customizable appearance
- Keyboard controls

## Controls
- **Arrow keys**: Move tiles
- **Space**: AI makes one move
- **A**: Toggle AI autoplay
- **N**: New game
- **Z**: Undo move

## Building
```bash
qmake6 2048_game.pro
make
./2048_game