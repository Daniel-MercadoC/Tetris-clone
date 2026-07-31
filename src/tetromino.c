#include <stdio.h>

#include "raylib.h"

#include "tetromino.h"

//
//    0 0 0 0    0 0 0 0    0 0 0 0    0 0 0 0
//    0 1 0 0    0 1 0 0    0 0 0 0    0 1 0 0
//    1 1 1 0    0 1 1 0    1 1 1 0    1 1 0 0
//    0 0 0 0    0 1 0 0    0 1 0 0    0 1 0 0
//

//
//    0 0 0 0 0 0 0 0
//    0 0 0 0 0 0 0 0
//    0 0 0 0 0 0 0 0
//    0 0 0 0 0 0 0 0
//    0 1 0 0 1 1 1 0
//    1 1 0 0 1 1 1 0
//    1 1 0 2 1 1 1 0
//    1 1 2 2 1 1 1 0
//    1 0 0 2 1 1 1 0
//

void TryRotateInDirection(ActivePiece *activePiece, bool isLeft) {
    ActivePiece rotatedPiece = *activePiece;
    if (isLeft) rotatedPiece.rotation = (((rotatedPiece.rotation - 1) % 4) + 4) % 4;
    else rotatedPiece.rotation = (rotatedPiece.rotation + 1) % 4;
    
    for (int i=0; i<NUM_KICKS; i++) {
        rotatedPiece.x = activePiece->x + KICK_OFFSETS[i].dx;
        rotatedPiece.y = activePiece->y + KICK_OFFSETS[i].dy;
        // printf("Checking offset: (%d, %d)\n", KICK_OFFSETS[i].dx, KICK_OFFSETS[i].dy);

        if (!OverlappingPieces(rotatedPiece)) {
            *activePiece = rotatedPiece;
            return;
        }
    }
}

void GenerateBottomCollisionTable(BottomCells *bottomCells) {
    bool foundX = false;
    
    for (int h=0; h<PIECE_COUNT; h++) {
        // printf("----------------------------------------\n");
        // printf("Piece%d: \n", h);
        for (int i=0; i<4; i++) {
            // printf("    Rotation%d: \n", i);
            for (int j=0; j<4; j++) {
                foundX = false;
                // printf("        Col%d: \n", j);
                for (int k=3; k>=0; k--) {
                    if (GetPieceCell(PIECE_DEFS[h].rotations[i], k, j)) {
                        bottomCells[h].lowestRow[i][j] = k;
                        // printf("            bottomCells[%d].coords[%d][%d] = %d\n", h, i, j, k);
                        foundX = true;
                    }
                    if (foundX) break;
                }
            }
        }
        // printf("----------------------------------------\n");
    }
}
