#ifndef RENDER_H
#define RENDER_H

#include "tetromino.h"
#include "game.h"

void DrawBoard(int x, int y, Color color);
void DrawPieces(const GameState gameState, int originX, int originY, Texture2D cell);
void DrawUI(const GameState gameState, int originX, int originY, Texture2D cell);

#endif
