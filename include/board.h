#ifndef BOARD
#define BOARD

#include "tetromino.h"

#define BOARD_W 10
#define BOARD_H 20
#define BOARD_BUFFER_H 4

typedef enum {
    CELL_EMPTY,
    CELL_I,
    CELL_J,
    CELL_L,
    CELL_O,
    CELL_S,
    CELL_T,
    CELL_Z
} CellType;
CellType board[BOARD_W][BOARD_H];

bool PieceCollision(ActivePiece activePiece);
bool CheckCollisionToSide(ActivePiece activePiece, bool isLeft);

#endif
