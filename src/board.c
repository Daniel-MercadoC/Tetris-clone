#include <stdio.h>

#include "raylib.h"

#include "board.h"

void UpdateBoard(ActivePiece activePiece) {
    for (int i=0; i<4; i++) {
        for (int j=0; j<4; j++) {
            board[activePiece.x+j][activePiece.y+i] =
                ((PIECE_DEFS[activePiece.type].rotations[activePiece.rotation]>>((i*4)+j)) & 1) * (activePiece.type+1);
            printf("board[%d][%d]: %d\n", activePiece.x+j, activePiece.y+i, board[activePiece.x+j][activePiece.y+i]);
        }
    }
}

bool PieceCollision(ActivePiece activePiece) {
    int cellsX[4] = {0};
    int lowestY = 0;
    // There will always be at least one element of any tetromino in the 3nd row, since rotations are centered in the 4x4 grid, so we can check from there.
    for (int i=2; i<4; i++) {
        if (((PIECE_DEFS[activePiece.type].rotations[activePiece.rotation]>>(4-i)*4) & 15) > 0) {
            // printf("result: %d\n", ((PIECE_DEFS[activePiece.type].rotations[activePiece.rotation]>>(4-i)*4) & 15));
            lowestY = i+1;
            printf("lowestY: %d\n", lowestY);
        }
    }
    for (int j=0; j<4; j++) {
        cellsX[j] = (PIECE_DEFS[activePiece.type].rotations[activePiece.rotation]>>(((lowestY-1)*4)+j) & 1);
        printf("cellsX[%d]: %d\n", j, cellsX[j]);
    }

    for (int e=0; e<4; e++) {
        if (cellsX[e]) {
            if (board[activePiece.x+e][activePiece.y+lowestY] > 0) {
                UpdateBoard(activePiece);
                return true;
            }
        }
    }
    if (activePiece.y+lowestY > BOARD_H) {
        UpdateBoard(activePiece);
        return true;
    }
        
    return false;
}
