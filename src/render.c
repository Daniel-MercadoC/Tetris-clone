#include <stdio.h>

#include "raylib.h"

#include "tetromino.h"
#include "board.h"
#include "render.h"
#include "style.h"

void DrawBoard(int x, int y, Color color) {
    for (int i=0; i<BOARD_W; i++) {
        DrawRectangleLines(x+(i*CELL_SIZE), y, CELL_SIZE, CELL_SIZE*(BOARD_H-BOARD_BUFFER_H), color);
    }
    for (int j=0; j<BOARD_H-BOARD_BUFFER_H; j++) {
        DrawRectangleLines(x, y+(j*CELL_SIZE), CELL_SIZE*BOARD_W, CELL_SIZE, color);
    }
}

void DrawPieces(const GameState gameState, int originX, int originY, Texture2D cell) {
    for (int y = BOARD_BUFFER_H; y < BOARD_H; y++) {
        for (int x = 0; x < BOARD_W; x++) {
            Color c = board[x][y] ? PIECE_DEFS[board[x][y]-1].color : BLACK;
            // DrawRectangle((x*CELL_SIZE)+originX, (y*CELL_SIZE)+originY, CELL_SIZE, CELL_SIZE, cell);
            DrawTexture(cell, ((x*CELL_SIZE)+originX), ((y*CELL_SIZE)+originY), c);
        }
    }
    
    // 4 is hard coded here because all pieces have 4 blocks, hence a tetromino
    uint16_t pieceRotation = PIECE_DEFS[gameState.activePiece.type].rotations[gameState.activePiece.rotation];
    // printf("pieceRotation: %d\n", pieceRotation);
    for (int i=0; i<4; i++) {
        for (int j=0; j<4; j++) {
            // printf("pieceRotation<<%d: %d\n", (i*4)+j, pieceRotation>>((i*4)+j) & 1);
            if (gameState.activePiece.y+i < BOARD_BUFFER_H) continue;
            if ((gameState.activePiece.x+j) < 0) continue;
            
            if (gameState.activePiece.y+i > BOARD_H-1) continue;
            if (gameState.activePiece.x+j > BOARD_W-1) continue;

            if (GetPieceCell(pieceRotation, i, j)) {
                // DrawRectangle(((gameState.activePiece.x+j)*CELL_SIZE)+originX, ((gameState.activePiece.y+i)*CELL_SIZE)+originY, CELL_SIZE, CELL_SIZE, PIECE_DEFS[gameState.activePiece.type].color);
                DrawTexture(cell, ((gameState.activePiece.x+j)*CELL_SIZE)+originX, ((gameState.activePiece.y+i)*CELL_SIZE)+originY, PIECE_DEFS[gameState.activePiece.type].color);
            }
        }
    }
}

void DrawUI(const GameState gameState, int originX, int originY, Texture2D cell) {
    // Since raylib draws rectangle borders from the center outwards (i.e. if the x origin is 0, the line will grow both to x+1 and x-1), the border of the board can end up drawn over pieces at the far left and far right.
    // To avoid this, the line width is subtracted from the x and y origins; which therefore needs to be added twice to the rectangle's size.
    DrawRectangleLinesEx(
    (Rectangle){ originX-(int)LINE_WIDTH,
                 originY-(int)LINE_WIDTH,
                 (CELL_SIZE*BOARD_W)+(int)LINE_WIDTH*2,
		 (CELL_SIZE*(BOARD_H-BOARD_BUFFER_H))+(int)LINE_WIDTH*2 },
        LINE_WIDTH,
        RAYWHITE);
	
    //------------------------------SCORE------------------------------
    DrawRectangleLinesEx(
    (Rectangle){ originX+(BOARD_W*CELL_SIZE)+BOX_SPACING,
                 originY-(int)LINE_WIDTH,
                 (CELL_SIZE*8)+TEXT_PADDING,
		 CELL_SIZE*3 },
        LINE_WIDTH,
        RAYWHITE);

    DrawText("SCORE:", originX+(BOARD_W*CELL_SIZE)+BOX_SPACING+TEXT_PADDING, originY-(int)LINE_WIDTH+TEXT_PADDING, TEXT_SIZE, RAYWHITE);
    DrawText(TextFormat("%010d", gameState.score), originX+(BOARD_W*CELL_SIZE)+BOX_SPACING+TEXT_PADDING, originY-(int)LINE_WIDTH+TEXT_SIZE+(TEXT_PADDING*2), TEXT_SIZE*2, RAYWHITE);

    DrawRectangleLinesEx(
    (Rectangle){ originX+(BOARD_W*CELL_SIZE)+BOX_SPACING,
                 originY-(int)LINE_WIDTH+(CELL_SIZE*3)+BOX_SPACING,
                 (CELL_SIZE*5)+TEXT_PADDING,
		 (CELL_SIZE*6)+TEXT_PADDING },
        LINE_WIDTH,
        RAYWHITE);

    //------------------------NEXT PIECE PREVIEW------------------------
    DrawText("NEXT PIECE:", originX+(BOARD_W*CELL_SIZE)+BOX_SPACING+TEXT_PADDING, originY-(int)LINE_WIDTH+(CELL_SIZE*3)+BOX_SPACING+TEXT_PADDING, TEXT_SIZE, RAYWHITE);
    
    uint16_t pieceRotation = PIECE_DEFS[gameState.nextPiece.type].rotations[gameState.nextPiece.rotation];
    for (int i=0; i<4; i++) {
        for (int j=0; j<4; j++) {
            if (GetPieceCell(pieceRotation, i, j)) {
                DrawTexture(cell,
		            originX+(BOARD_W*CELL_SIZE)+BOX_SPACING+TEXT_PADDING+(j*CELL_SIZE),
		            originY-(int)LINE_WIDTH+(CELL_SIZE*3)+BOX_SPACING+TEXT_SIZE+(TEXT_PADDING*2)+(i*CELL_SIZE),
			    PIECE_DEFS[gameState.nextPiece.type].color);
            }
        }
    }

    //------------------------------LEVEL------------------------------
    DrawRectangleLinesEx(
    (Rectangle){ originX+(BOARD_W*CELL_SIZE)+BOX_SPACING,
                 originY-(int)LINE_WIDTH+(CELL_SIZE*9)+(BOX_SPACING*2)+TEXT_PADDING,
                 (CELL_SIZE*5)+TEXT_PADDING,
		 CELL_SIZE*3 },
        LINE_WIDTH,
        RAYWHITE);

    DrawText("LEVEL:", originX+(BOARD_W*CELL_SIZE)+BOX_SPACING+TEXT_PADDING, originY-(int)LINE_WIDTH+(CELL_SIZE*9)+(BOX_SPACING*2)+(TEXT_PADDING*2), TEXT_SIZE, RAYWHITE);
    DrawText(TextFormat("%d", gameState.level), originX+(BOARD_W*CELL_SIZE)+BOX_SPACING+TEXT_PADDING, originY-(int)LINE_WIDTH+(CELL_SIZE*9)+(BOX_SPACING*2)+TEXT_SIZE+(TEXT_PADDING*3), TEXT_SIZE*2, RAYWHITE);
}
