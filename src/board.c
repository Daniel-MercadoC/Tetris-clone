#include <stdio.h>

#include "raylib.h"

#include "board.h"
#include "tetromino.h"

typedef struct {
    int x, y;
} CellCoords;

void UpdateBoard(ActivePiece activePiece) {
    for (int i=0; i<4; i++) {
        for (int j=0; j<4; j++) {
            board[activePiece.x+j][activePiece.y+i] =
                ((GetPieceCell(PIECE_DEFS[activePiece.type].rotations[activePiece.rotation], i, j)) * (activePiece.type+1));
            printf("board[%d][%d]: %d\n", activePiece.x+j, activePiece.y+i, board[activePiece.x+j][activePiece.y+i]);
        }
    }
}

bool PieceCollision(ActivePiece activePiece) {
    CellCoords cells[4] = {0};
    
    // for (int i=0; j<4; j++) {
    //     cells[i].x = (GetPieceCell(PIECE_DEFS[activePiece.type].rotations[activePiece.rotation], i, 0));
    //     cells[i].y = (GetPieceCell(PIECE_DEFS[activePiece.type].rotations[activePiece.rotation], i, 0));
    // }

    for (int e=0; e<4; e++) {
        if (cellsX[e]) {
            if (board[activePiece.x+e][activePiece.y+lowestY+1] > 0) {
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
