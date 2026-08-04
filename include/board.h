#ifndef BOARD
#define BOARD

#include "tetromino.h"

#define BOARD_W 10
#define BOARD_H 24
#define BOARD_BUFFER_H 4
#define BOARD_H_LIMIT 22

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

void UpdateBoard(const ActivePiece activePiece);
bool BottomCollision(const ActivePiece activePiece, const BottomCells *bottomCells);
bool OverlappingPieces(const ActivePiece activePiece);
bool CollisionToSide(const ActivePiece activePiece, bool isLeft);
bool HeightLimitReached();
int ClearFullRows();
void ResetBoard();

#endif
