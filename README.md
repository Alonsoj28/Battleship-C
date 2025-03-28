# Battleship Game

## Overview
This is a console-based Battleship game implemented in C, where you can play against a computer opponent on a customizable grid. Choose your game mode, board size, and engage in a strategic naval battle!

## Game Modes
### 1. Didactic Mode
- Displays both player and opponent boards
- Provides more visual feedback during gameplay

### 2. Easy Mode
- Hides opponent's board
- More challenging gameplay experience

## How to Play

### Game Setup
1. When you start the game, you'll see a main menu with options:
   - Modify board dimension
   - Select game mode
   - Start game
   - Exit

### Board Configuration
- Default board size is 10x10
- You can customize the board size before starting the game
- Boards will be randomly populated with ships

### Ship Types
- Lancha (Boat): 1 cell
- Crucero (Cruiser): 2 cells
- Submarino (Submarine): 3 cells
- Buque (Battleship): 4 cells
- Portaviones (Aircraft Carrier): 5 cells

### Gameplay
- Ships are placed randomly on both boards
- Players take turns attacking coordinates
- First to sink all opponent's ships wins!

## Attacking
- Enter X and Y coordinates to attack
- For example, if you wanted to attack (4, 3), the expected input should be: "4 3"
- Coordinate system starts at (0,0) in the bottom-left corner
- Hit: 'X' marks the spot
- Miss: '-' remains on the board

## Compilation
```bash
gcc battleship.c -o battleship
./battleship
```

## Game Rules
- Ships are placed randomly at game start
- Game ends when all ships of one player are sunk
- Coordinates are zero-indexed
- Board occupation is limited to 30-35% of total board cells

## Game Flow
1. Choose board size
2. Select game mode
3. Random ship placement
4. Alternate turns between player and computer
5. Attack enemy ships
6. First to sink all ships wins!

## Tips
- Pay attention to your board layout
- Try different board sizes for varied gameplay
- Practice both game modes to improve strategy

## Supported Platforms
- Linux (uses `system("clear")`)
- May require minor modifications for Windows
- Recommended to play in Replit.com for Windows users

## Notes
- Game uses random seed based on current time
- Randomized ship placement ensures unique games
