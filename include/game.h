#ifndef GAME_H
#define GAME_H

#include "tetromino.h"

typedef enum {
    SCREEN_MENU,
    SCREEN_GAME,
    SCREEN_GAMEOVER
} GameScreen;

typedef struct {
    GameScreen currentScreen;
    ActivePiece activePiece;
    ActivePiece nextPiece;
    int score;
    int totalRows;
    int level;
} GameState;

// This is a table based on the original games' speeds, multiplied by two since the framerate of this version is twice the original.
static const int PIECE_SPEEDS[30] = {
    [0] = 96,
    [1] = 86,
    [2] = 76,
    [3] = 66,
    [4] = 56,
    [5] = 46,
    [6] = 36,
    [7] = 26,
    [8] = 16,
    [9] = 12,
    [10] = 10,
    [11] = 10,
    [12] = 10,
    [13] = 8,
    [14] = 8,
    [15] = 8,
    [16] = 6,
    [17] = 6,
    [18] = 6,
    [19] = 4,
    [20] = 4,
    [21] = 4,
    [22] = 4,
    [23] = 4,
    [24] = 4,
    [25] = 4,
    [26] = 4,
    [27] = 4,
    [28] = 4,
    [29] = 2
};

int DeterminePieceSpeed(const GameState gameState, bool softDrop);
void UpdateLevel(GameState *gameState);
void UpdateScore(GameState *gameState, int clearedRows, int softDroppedCells);
int LockPieceWithDelay(GameState *gameState, const int frameFlipCounter, const int frameFlipThreshold, const int softDropStart);

#endif
