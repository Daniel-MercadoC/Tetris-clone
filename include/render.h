#ifndef RENDER_H
#define RENDER_H

#include "raylib.h"

#include "tetromino.h"
#include "game.h"

// Image button was added here since more than one file (main.c and ui.c) needs access to its components.
typedef struct {
    Texture2D tex;
    Rectangle bounds;
    float baseScale;
    float hoverScale;
    int id;
} ImageButton;

void DrawBoard();
void DrawPieces(const GameState gameState, Texture2D cell);
void DrawUI(const GameState gameState, Texture2D cell);

#endif
