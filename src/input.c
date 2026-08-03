#include "raylib.h"

#include "input.h"

void CheckForPieceMovement(GameState *gameState, int *heldFrameCounter, const int dasThreshold, const int arrValue) {
    if (IsKeyPressed(KEY_LEFT) && !IsKeyDown(KEY_RIGHT)) {
            if (!CollisionToSide(gameState->activePiece, true)) gameState->activePiece.x -= 1;
        }
	if (IsKeyDown(KEY_LEFT) && !IsKeyDown(KEY_RIGHT)) {
	    *heldFrameCounter += 1;
	    if (*heldFrameCounter >= dasThreshold && (*heldFrameCounter % arrValue == 0)) {
                if (!CollisionToSide(gameState->activePiece, true)) gameState->activePiece.x -= 1;
	    }
	}
        if (IsKeyPressed(KEY_RIGHT) && !IsKeyDown(KEY_LEFT)) {
            if (!CollisionToSide(gameState->activePiece, false)) gameState->activePiece.x += 1;
        }
	if (IsKeyDown(KEY_RIGHT) && !IsKeyDown(KEY_LEFT)) {
	    *heldFrameCounter += 1;
	    if (*heldFrameCounter >= dasThreshold && (*heldFrameCounter % arrValue == 0)) {
                if (!CollisionToSide(gameState->activePiece, false)) gameState->activePiece.x += 1;
	    }
	}
	if (IsKeyUp(KEY_LEFT) && IsKeyUp(KEY_RIGHT)) *heldFrameCounter = 0;
}

void CheckForSoftDrop(GameState *gameState, int *softDropStart, int *pieceDropThreshold) {
    if (IsKeyPressed(KEY_DOWN)) *softDropStart = gameState->activePiece.x;
        if (IsKeyDown(KEY_DOWN)) {
            *pieceDropThreshold = DeterminePieceSpeed(*gameState, true);
        }
        if (IsKeyUp(KEY_DOWN)) {
	    *pieceDropThreshold = DeterminePieceSpeed(*gameState, false);
	    *softDropStart = 19;
	}
}

void CheckForRotation(ActivePiece *activePiece) {
    if (IsKeyPressed(KEY_SPACE)) {
        TryRotateInDirection(activePiece, false);
    }
    if (IsKeyPressed(KEY_Z)) {
        TryRotateInDirection(activePiece, true);
    }
}
