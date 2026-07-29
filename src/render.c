// #include <stdio.h>

#include "raylib.h"

#include "tetromino.h"
#include "board.h"
#include "render.h"

void DrawBoard(int x, int y, int cellSize, Color color) {
    for (int i=0; i<BOARD_W; i++) {
        DrawRectangleLines(x+(i*cellSize), y, cellSize, cellSize*(BOARD_H-BOARD_BUFFER_H), color);
    }
    for (int j=0; j<BOARD_H-BOARD_BUFFER_H; j++) {
        DrawRectangleLines(x, y+(j*cellSize), cellSize*BOARD_W, cellSize, color);
    }
}

void DrawPiece(int originX, int originY, ActivePiece activePiece, int cellSize) {
    // 4 is hard coded here because all pieces have 4 blocks, hence a tetromino
    uint16_t pieceRotation = PIECE_DEFS[activePiece.type].rotations[activePiece.rotation];
    // printf("pieceRotation: %d\n", pieceRotation);
    for (int i=0; i<4; i++) {
        for (int j=0; j<4; j++) {
            // printf("pieceRotation<<%d: %d\n", (i*4)+j, pieceRotation>>((i*4)+j) & 1);
            if (activePiece.y+i < BOARD_BUFFER_H) continue;
            if (GetPieceCell(pieceRotation, i, j)) {
                DrawRectangle(((activePiece.x+j)*cellSize)+originX, ((activePiece.y+i)*cellSize)+originY, cellSize, cellSize, PIECE_DEFS[activePiece.type].color);
            } else if (!board[activePiece.x+j][activePiece.y+i+1]) {
                DrawRectangle(((activePiece.x+j)*cellSize)+originX, ((activePiece.y+i)*cellSize)+originY, cellSize, cellSize, BLACK);
            }
        }
    }
    DrawRectangle(((activePiece.x)*cellSize)+originX, ((activePiece.y-1)*cellSize)+originY, 4*cellSize, cellSize, BLACK);
}
