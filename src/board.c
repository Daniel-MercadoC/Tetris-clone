#include <stdio.h>

#include "raylib.h"

#include "board.h"
#include "tetromino.h"

// typedef struct {
//     int x, y;
// } CellCoords;


void PrintBoard() {
    printf("--------BOARD--------\n");
    for (int i=0; i<BOARD_H; i++) {
        for (int j=0; j<BOARD_W; j++) {
        printf("%d", board[j][i]);
        }
        printf("\n");
    }
    printf("---------------------\n");
}

void UpdateBoard(ActivePiece activePiece) {
    for (int i=0; i<4; i++) {
        for (int j=0; j<4; j++) {
            if ((activePiece.x+j) < 0) continue;
            if (!GetPieceCell(PIECE_DEFS[activePiece.type].rotations[activePiece.rotation], i, j)) continue;
            // printf("----------------\n");
            // printf("Active piece: \n");
            // printf("x: %d\n", activePiece.x+j);
            // printf("y: %d\n", activePiece.y+i);
            // printf("cell: %d\n", GetPieceCell(PIECE_DEFS[activePiece.type].rotations[activePiece.rotation], i, j));
            // printf("----------------\n");
            board[activePiece.x+j][activePiece.y+i] = activePiece.type+1;
        }
    }
    PrintBoard();
}

bool BottomCollision(ActivePiece activePiece, const BottomCells *bottomCells) {
        // printf("------------------------------\n");
        // printf("Cells for current rotation\n");
    for (int i=0; i<4; i++) {
        // printf("At: \n");
        int y = bottomCells[activePiece.type].lowestRow[activePiece.rotation][i];
        // printf("    bottomCells[%d].coords[%d][%d]: %d\n", activePiece.type, activePiece.rotation, i, y);
        
        // There's no piece which at any rotation fills its lowest cell at y = 0, therefore for any filled cell a y different from 0.
        if (!y) continue;

        if ((activePiece.y + y) >= BOARD_H-1) return true;
        if (board[activePiece.x+i][activePiece.y + (y+1)] > 0) return true;
    }
        // printf("------------------------------\n");
        
    return false;
}

bool CollisionToSide(ActivePiece activePiece, bool isLeft) {
    int dir = isLeft ? -1 : 1;
    for (int i=0; i<4; i++) {
        for (int j=0; j<4; j++) {
            if (!GetPieceCell(PIECE_DEFS[activePiece.type].rotations[activePiece.rotation], i, j)) continue;
            
            int checkX = activePiece.x + j + dir;
            if ((checkX < 0) || (checkX > BOARD_W-1)) return true;
            if (board[checkX][activePiece.y+i]) return true;
        }
    }
    return false;
}

bool OverlappingPieces(ActivePiece activePiece) {
    for (int i=0; i<4; i++) {
        for (int j=0; j<4; j++) {
            if (GetPieceCell(PIECE_DEFS[activePiece.type].rotations[activePiece.rotation], i, j)) {
                if (activePiece.x+j > BOARD_W-1) return true;
                if (activePiece.y+i > BOARD_H-1) return true;
                
                if (activePiece.x+j < 0) return true;
                if (activePiece.y+i < 0) return true;
                
                if (board[activePiece.x+j][activePiece.y+i]) {
                    // printf("Overlapping pieces detected at: (%d, %d)\n", activePiece.x+j, activePiece.y+i);
                    return true;
                }
            }
        }
    }
    // printf("No overlapping pieces when at: (%d, %d)\n", activePiece.x, activePiece.y);
    return false;
}

void CopyRow(int read, int write) {
    for (int cols=0; cols<BOARD_W; cols++) {
        board[cols][write] = board[cols][read];
    }
}

void ClearFullRows() {
    int write = BOARD_H-1;

    for (int rows=BOARD_H-1; rows>=BOARD_BUFFER_H; rows--) {
        for (int cols=0; cols<BOARD_W; cols++) {
            int value = board[cols][rows];
            
            if (value == 0) {
                // if (read != write)
                CopyRow(rows, write);
                write -= 1;
                break;
            }
        }
    }

    for (int rows=BOARD_BUFFER_H; rows<=write; rows++) {
        for (int cols=0; cols<BOARD_W; cols++) {
            board[cols][rows] = 0;
        }
    }
}

