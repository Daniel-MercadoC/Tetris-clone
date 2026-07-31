#include <stdio.h>

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

void DrawPieces(int originX, int originY, ActivePiece activePiece, int cellSize) {
    for (int y = BOARD_BUFFER_H; y < BOARD_H; y++) {
        for (int x = 0; x < BOARD_W; x++) {
            Color cell = board[x][y] ? PIECE_DEFS[board[x][y]-1].color : BLACK;
            DrawRectangle((x*cellSize)+originX, (y*cellSize)+originY, cellSize, cellSize, cell);
        }
    }
    
    // 4 is hard coded here because all pieces have 4 blocks, hence a tetromino
    uint16_t pieceRotation = PIECE_DEFS[activePiece.type].rotations[activePiece.rotation];
    // printf("pieceRotation: %d\n", pieceRotation);
    for (int i=0; i<4; i++) {
        for (int j=0; j<4; j++) {
            // printf("pieceRotation<<%d: %d\n", (i*4)+j, pieceRotation>>((i*4)+j) & 1);
            if (activePiece.y+i < BOARD_BUFFER_H) continue;
            if ((activePiece.x+j) < 0) continue;
            
            if (activePiece.y+i > BOARD_H-1) continue;
            if (activePiece.x+j > BOARD_W-1) continue;

            if (GetPieceCell(pieceRotation, i, j)) {
                DrawRectangle(((activePiece.x+j)*cellSize)+originX, ((activePiece.y+i)*cellSize)+originY, cellSize, cellSize, PIECE_DEFS[activePiece.type].color);
            }
        }
    }
}
