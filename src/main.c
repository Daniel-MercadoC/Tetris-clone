#include <stdio.h>

#include "raylib.h"

#include "board.h"
#include "tetromino.h"
#include "render.h"

int main() {
    int screenWidth = 1080;
    int screenHeight = 720;
    
    const int cellSize = 32;
    const int boardOriginX = (screenWidth/2) - (BOARD_W*cellSize/2);
    const int boardOriginY = (screenHeight/2) - ((BOARD_H-BOARD_BUFFER_H)*cellSize/2);

    float timerToFall = 0.0;
    const float timerToFallThreshold = 0.3;

    bool atBottom = false;
    float timerToFlip = 0.0;
    const float timerToFlipThreshold = 1.0;

    ActivePiece activePiece = {
        .type = GetRandomValue(0, 6),
        .rotation = 0,
        .x = 2-2,
        .y = 0
    };

    ActivePiece newPiece = {
        .type = GetRandomValue(0, 6),
        .rotation = 0,
        .x = 2-2,
        .y = 0
    };

    // There are 4 rotations per piece and 4 possible x coordinates per rotation.
    BottomCells bottomCells[PIECE_COUNT] = {0};
    GenerateBottomCollisionTable(bottomCells);
    
    InitWindow(screenWidth, screenHeight, "Tetris");

    SetTargetFPS(120);

    
    while (!WindowShouldClose()) {
        if (IsKeyPressed(KEY_SPACE)) {
            TryRotateInDirection(&activePiece, false);
        }
        if (IsKeyPressed(KEY_Z)) {
            TryRotateInDirection(&activePiece, true);
        }
        if (IsKeyPressed(KEY_LEFT)) {
            if (!CollisionToSide(activePiece, true)) activePiece.x -= 1;
        }
        if (IsKeyPressed(KEY_RIGHT)) {
            if (!CollisionToSide(activePiece, false)) activePiece.x += 1;
        }

        // printf("frame time: %f\n", GetFrameTime());
        // printf("timerToFall: %f\n", timerToFall);
        
        if (BottomCollision(activePiece, bottomCells)) {
            atBottom = true;
        } else {
            atBottom = false;
        }

        if (atBottom) {
            timerToFlip += GetFrameTime();
            if (timerToFlip >= timerToFlipThreshold) {   
                UpdateBoard(activePiece);
                ClearFullRows();
                newPiece.type = GetRandomValue(0, 6);
                // newPiece.type = 5;
                activePiece = newPiece;
                timerToFlip -= timerToFlipThreshold;
            }
        } else {
            timerToFlip = 0;
            timerToFall += GetFrameTime();
            if (timerToFall >= timerToFallThreshold) {
                activePiece.y += 1;
                timerToFall -= timerToFallThreshold;
            }
        }
        
        BeginDrawing();
        ClearBackground(BLACK);
        DrawPieces(boardOriginX, boardOriginY-(BOARD_BUFFER_H*cellSize), activePiece, cellSize);
        DrawBoard(boardOriginX, boardOriginY, cellSize, GRAY);
        EndDrawing();
    }

    CloseWindow();

    return 0;
}
