#include <stdio.h>

#include "raylib.h"

#include "board.h"
#include "game.h"

int DeterminePieceSpeed(const GameState gameState, bool softDrop) {
    int speed = 0;
    
    if (softDrop) speed = (gameState.level < 29) ? PIECE_SPEEDS[28] : PIECE_SPEEDS[gameState.level];
    else if (gameState.level < 29) speed = PIECE_SPEEDS[gameState.level];
    else speed = PIECE_SPEEDS[29];

    return speed;
}

void UpdateLevel(GameState *gameState) {
    if (gameState->totalRows - ((gameState->level+1)*10) >= 0) gameState->level += 1;
}

void UpdateScore(GameState *gameState, int clearedRows, int softDroppedCells) {
    if (clearedRows == 1) gameState->score +=   40 * (gameState->level + 1);
    if (clearedRows == 2) gameState->score +=  100 * (gameState->level + 1);
    if (clearedRows == 3) gameState->score +=  300 * (gameState->level + 1);
    if (clearedRows == 4) gameState->score += 1200 * (gameState->level + 1);
    if (softDroppedCells > 0) gameState->score += softDroppedCells;
    return;
}

void ManageGameState(GameState *gameState, bool startGame, bool triggerGameOver, bool resetGame) {
    if (gameState->currentScreen == SCREEN_MENU && startGame) gameState->currentScreen = SCREEN_GAME;
    if (gameState->currentScreen == SCREEN_GAME && triggerGameOver) gameState->currentScreen = SCREEN_GAMEOVER;
    if (gameState->currentScreen == SCREEN_GAMEOVER && resetGame) gameState->currentScreen = SCREEN_GAME;
}

int LockPieceWithDelay(GameState *gameState, const int frameFlipCounter, const int frameFlipThreshold, const int softDropStart) {
    if (frameFlipCounter >= frameFlipThreshold) {
	UpdateBoard(gameState->activePiece);
	
	int clearedRows = ClearFullRows();
	
	UpdateScore(gameState, clearedRows, (gameState->activePiece.y)-softDropStart);
	if (clearedRows > 0) {
	    gameState->totalRows += clearedRows;

	    UpdateLevel(gameState);
	}
	gameState->activePiece = gameState->nextPiece;
	// nextPiece.type = 5;
	gameState->nextPiece.type = GetRandomValue(0, 6);
	return 1;
    }
    return 0;
}
