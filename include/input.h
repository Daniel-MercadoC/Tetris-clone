#ifndef INPUT_H
#define INPUT_H

#include "tetromino.h"
#include "game.h"

void CheckForPieceMovement(GameState *gameState, int *heldFrameCounter, const int dasThreshold, const int arrValue);
void CheckForSoftDrop(GameState *gameState, int *softDropStart, int *pieceDropThreshold);
void CheckForRotation(ActivePiece *activePiece);

#endif
