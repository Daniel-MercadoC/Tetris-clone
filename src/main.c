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
    const float timerToFallThreshold = 0.8;

    ActivePiece activePiece = {
        .type = 0,
        .rotation = 0,
        .x = 2-2,
        .y = 0
    };

    ActivePiece newPiece = {
        .type = 0,
        .rotation = 1,
        .x = 6-2,
        .y = 0
    };
    
    InitWindow(screenWidth, screenHeight, "Tetris");

    SetTargetFPS(120);

    BeginDrawing();
    ClearBackground(BLACK);
    DrawBoard(boardOriginX, boardOriginY, cellSize, GRAY);
    EndDrawing();
    
    while (!WindowShouldClose()) {
        if (IsKeyPressed(KEY_SPACE)) activePiece.rotation = (activePiece.rotation + 1) % 4;

        // printf("frame time: %f\n", GetFrameTime());
        // printf("timerToFall: %f\n", timerToFall);
        timerToFall += GetFrameTime();
        if (timerToFall >= timerToFallThreshold) {
            activePiece.y += 1;
            if (PieceCollision(activePiece)) {
                activePiece = newPiece;
            }
            timerToFall -= timerToFallThreshold;
        }
        
        BeginDrawing();
        DrawPiece(boardOriginX, boardOriginY-(BOARD_BUFFER_H*cellSize), activePiece, cellSize);
        DrawBoard(boardOriginX, boardOriginY, cellSize, GRAY);
        EndDrawing();
    }

    CloseWindow();

    return 0;
}
