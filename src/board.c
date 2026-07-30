#include <stdio.h>

#include "raylib.h"

#include "board.h"
#include "tetromino.h"

typedef struct {
    int x, y;
} CellCoords;


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
            board[activePiece.x+j][activePiece.y+i-1] = activePiece.type+1;
        }
    }
    // PrintBoard();
}

bool PieceCollision(ActivePiece activePiece) {
    CellCoords cells[4] = {0};
    int lowestY = 2;
    
    for (int i=3; i>=0; i--) {
	for (int j=0; j<4; j++) {
	    if (GetPieceCell(PIECE_DEFS[activePiece.type].rotations[activePiece.rotation], i, j) && (cells[j].x < 1)) {
		cells[j].x = j+1;
		cells[j].y = i;
		if (i > lowestY) lowestY = i;
	    }
	}
    }

    for (int e=0; e<4; e++) {
        if (cells[e].x > 0) {
            if (board[activePiece.x+e][activePiece.y+cells[e].y] > 0) {
                UpdateBoard(activePiece);
                return true;
            }
        }
    }
    if (activePiece.y+lowestY+1 > BOARD_H) {
        UpdateBoard(activePiece);
        return true;
    }
        
    return false;
}
