#ifndef RENDER_H
#define RENDER_H

#include "tetromino.h"

void DrawBoard(int x, int y, int cellSize, Color color);
void DrawPiece(int originX, int originY, ActivePiece activePiece, int cellSize);

#endif
