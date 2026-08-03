#include <stdio.h>

#include "raylib.h"

#include "board.h"
#include "game.h"
#include "input.h"
#include "tetromino.h"
#include "render.h"
#include "style.h"

int main() {
    
    const int boardOriginX = (SCREEN_WIDTH/2) - (BOARD_W*CELL_SIZE/2);
    const int boardOriginY = (SCREEN_HEIGHT/2) - ((BOARD_H-BOARD_BUFFER_H)*CELL_SIZE/2);

    ActivePiece activePiece = {
        .type = GetRandomValue(0, 6),
        .rotation = 0,
        .x = 0,
        .y = 0
    };

    ActivePiece nextPiece = {
        .type = GetRandomValue(0, 6),
        .rotation = 0,
        .x = 0,
        .y = 0
    };
    
    GameState gameState = {
	.currentScreen = SCREEN_GAME,
	.activePiece = activePiece,
	.nextPiece = nextPiece,
	.score = 0,
	.totalRows = 0,
	.level = 0
    };
    
    // float timerToFall = 0.0;
    // const float timerToFallThreshold = 0.3;
    int frameCounter = 0;
    int pieceDropThreshold = PIECE_SPEEDS[gameState.level];

    int frameFlipCounter = 0;
    const int frameFlipThreshold = 48;
    
    int heldFrameCounter = 0;
    const int dasFrameThreshold = 32;
    const int arrFrameValue = 12;

    int softDropStart = 19;

    // There are 4 rotations per piece and 4 possible x coordinates per rotation.
    BottomCells bottomCells[PIECE_COUNT] = {0};
    GenerateBottomCollisionTable(bottomCells);
    
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Tetris");

    Texture2D cell = LoadTexture("res/Block.png");
    
    SetTargetFPS(120);

    
    while (!WindowShouldClose()) {
        
        CheckForRotation(&gameState.activePiece);
        CheckForPieceMovement(&gameState, &heldFrameCounter, dasFrameThreshold, arrFrameValue);
	CheckForSoftDrop(&gameState, &softDropStart, &pieceDropThreshold);
	
        if (BottomCollision(gameState.activePiece, bottomCells)) {
	    frameFlipCounter++;
	    // printf("Piece at bottom\n");
	    printf("Frame flip counter: %d\n", frameFlipCounter);
            if (LockPieceWithDelay(&gameState, frameFlipCounter, frameFlipThreshold, softDropStart) > 0) {
	        printf("Piece locked\n");
	        frameFlipCounter = 0;
	    }
        } else {
	    frameFlipCounter = 0;
            if (frameCounter >= pieceDropThreshold) {
                gameState.activePiece.y += 1;
                frameCounter = 0;
            }
        }

	BeginDrawing();
	ClearBackground(BLACK);
	// ManageGameState(&gameState, false, false, false);
	DrawPieces(gameState, boardOriginX, boardOriginY-(BOARD_BUFFER_H*CELL_SIZE), cell);
        // The color is raylib's dark gray, but more transparent to make the board less noisy
        DrawBoard(boardOriginX, boardOriginY, (Color){ 80, 80, 80, 40 });
	DrawUI(gameState, boardOriginX, boardOriginY, cell);
        EndDrawing();

	frameCounter++;
    }
    UnloadTexture(cell);

    CloseWindow();

    return 0;
}
