#include <stdio.h>

#include "raylib.h"

#include "board.h"
#include "render.h"
#include "style.h"

#define ORIGIN_X (SCREEN_WIDTH/2) - (BOARD_W*CELL_SIZE/2)
#define ORIGIN_Y (SCREEN_HEIGHT/2) - ((BOARD_H-BOARD_BUFFER_H)*CELL_SIZE/2)

#define MOUSE_DEAD_ZONE 6.0f

static bool UpdateImageButton(ImageButton *btn, const bool highlighted) {
    // When the button is highlighted, it'll scale up. When it's pressed, it'll shrink back to normal.
    bool pressed = highlighted && IsMouseButtonDown(MOUSE_LEFT_BUTTON);

    float scaledWidth = btn->bounds.width * ((highlighted && !pressed) ? btn->hoverScale : btn->baseScale);
    float scaledHeight = btn->bounds.height * ((highlighted && !pressed) ? btn->hoverScale : btn->baseScale);
    float x = btn->bounds.x + (btn->bounds.width - scaledWidth) / 2;
    float y = btn->bounds.y + (btn->bounds.height - scaledHeight) / 2;

    // Button needs to be redrawn to a slightly different coordinate to keep its central position.
    DrawTexturePro(btn->tex,
                   (Rectangle){0, 0, btn->tex.width, btn->tex.height},
                   (Rectangle){x, y, scaledWidth, scaledHeight},
                   (Vector2){0, 0}, 0.0f,
                   WHITE
                   );

    return pressed;
}

static void UpdateMenuSelection(GameState *gameState, ImageButton *menuButtons, const size_t buttonCount) {
    // This prevents sudden movements (likely made accidentaly) to be detected in changing menu selection. The 6.0f represents the amount of pixels per frame allowed, which
    //   makes it so the user has to purposefully move the cursor pretty fast to not be detected.
    //
    // It could be better handled by calculating a minimum distance and maximum speed, but those are finer details outside of the scope of a simple project such as this one.

    Vector2 mouseDelta = GetMouseDelta();
    if ((
         ( (mouseDelta.x > 0.0f) && (mouseDelta.x <  MOUSE_DEAD_ZONE) ) ||
         ( (mouseDelta.x < 0.0f) && (mouseDelta.x > -MOUSE_DEAD_ZONE) ) ||
         ( (mouseDelta.y > 0.0f) && (mouseDelta.y <  MOUSE_DEAD_ZONE) ) ||
         ( (mouseDelta.y < 0.0f) && (mouseDelta.y > -MOUSE_DEAD_ZONE) )
       )) {

        gameState->selectedButton = -1;

        // This check is made first to prevent having to make a second pass after rendering the buttons, which would also mean a re-draw.
        for (size_t i=0; i<buttonCount; i++) {
            if (CheckCollisionPointRec(GetMousePosition(), menuButtons[i].bounds)) {
                gameState->selectedButton = i;
            } 
        }
    }
}

int DrawMenu(GameState *gameState, ImageButton *menuButtons, const size_t buttonCount) {
    int buttonPressed = 0;

    UpdateMenuSelection(gameState, menuButtons, buttonCount);
    
    ClearBackground(BLACK);

    // Main menu consists of title and buttons.
    DrawText("PONG",
             (SCREEN_WIDTH/2) - (MeasureText("PONG", TITLE_SIZE)/2),
             TITLE_PADDING,
             TITLE_SIZE,
             RAYWHITE
             );

    for (size_t i=0; i<buttonCount; i++) {
        bool highlighted = (gameState->selectedButton == i);
        if (UpdateImageButton(&menuButtons[i], highlighted)) {
            buttonPressed = menuButtons[i].id;
        }
    }
    
    // The function returns the corresponding id of the button pressed, if any.
    return buttonPressed;
}

void DrawBoard() {
    // The color is raylib's dark gray, but more transparent to make the board less noisy
    const Color color = (Color){ 80, 80, 80, 40 };
    
    for (int rows=0; rows<BOARD_W; rows++) {
        DrawRectangleLines(ORIGIN_X+(rows*CELL_SIZE), ORIGIN_Y, CELL_SIZE, CELL_SIZE*(BOARD_H-BOARD_BUFFER_H), color);
    }
    for (int cols=0; cols<BOARD_H-BOARD_BUFFER_H; cols++) {
        DrawRectangleLines(ORIGIN_X, ORIGIN_Y+(cols*CELL_SIZE), CELL_SIZE*BOARD_W, CELL_SIZE, color);
    }
}

void DrawPieces(const GameState gameState, Texture2D cell) {
    const int y = ORIGIN_Y - (BOARD_BUFFER_H*CELL_SIZE);

    for (int rows = BOARD_BUFFER_H; rows < BOARD_H; rows++) {
        for (int cols = 0; cols < BOARD_W; cols++) {
            Color c = board[cols][rows] ? PIECE_DEFS[board[cols][rows]-1].color : BLACK;
            // DrawRectangle((cols*CELL_SIZE)+ORIGIN_X, (rows*CELL_SIZE)+y, CELL_SIZE, CELL_SIZE, cell);
            DrawTexture(cell, ((cols*CELL_SIZE)+ORIGIN_X), ((rows*CELL_SIZE)+y), c);
        }
    }
    
    // 4 is hard coded here because all pieces have 4 blocks, hence a tetromino
    uint16_t pieceRotation = PIECE_DEFS[gameState.activePiece.type].rotations[gameState.activePiece.rotation];
    // printf("pieceRotation: %d\n", pieceRotation);
    for (int rows=0; rows<4; rows++) {
        for (int cols=0; cols<4; cols++) {
            // printf("pieceRotation<<%d: %d\n", (rows*4)+cols, pieceRotation>>((rows*4)+cols) & 1);
            if (gameState.activePiece.y+rows < BOARD_BUFFER_H) continue;
            if ((gameState.activePiece.x+cols) < 0) continue;
            
            if (gameState.activePiece.y+rows > BOARD_H-1) continue;
            if (gameState.activePiece.x+cols > BOARD_W-1) continue;

            if (GetPieceCell(pieceRotation, rows, cols)) {
                // DrawRectangle(((gameState.activePiece.x+cols)*CELL_SIZE)+ORIGIN_X, ((gameState.activePiece.y+rows)*CELL_SIZE)+y, CELL_SIZE, CELL_SIZE, PIECE_DEFS[gameState.activePiece.type].color);
                DrawTexture(cell, ((gameState.activePiece.x+cols)*CELL_SIZE)+ORIGIN_X, ((gameState.activePiece.y+rows)*CELL_SIZE)+y, PIECE_DEFS[gameState.activePiece.type].color);
            }
        }
    }
}

void DrawUI(const GameState gameState, Texture2D cell) {
    // Since raylib draws rectangle borders from the center outwards (i.e. if the x origin is 0, the line will grow both to x+1 and x-1), the border of the board can end up drawn over pieces at the far left and far right.
    // To avoid this, the line width is subtracted from the x and y origins; which therefore needs to be added twice to the rectangle's size.
    DrawRectangleLinesEx(
    (Rectangle){ ORIGIN_X-(int)LINE_WIDTH,
                 ORIGIN_Y-(int)LINE_WIDTH,
                 (CELL_SIZE*BOARD_W)+(int)LINE_WIDTH*2,
		 (CELL_SIZE*(BOARD_H-BOARD_BUFFER_H))+(int)LINE_WIDTH*2 },
        LINE_WIDTH,
        RAYWHITE);
	
    //------------------------------SCORE------------------------------
    DrawRectangleLinesEx(
    (Rectangle){ ORIGIN_X+(BOARD_W*CELL_SIZE)+BOX_SPACING,
                 ORIGIN_Y-(int)LINE_WIDTH,
                 (CELL_SIZE*8)+TEXT_PADDING,
		 CELL_SIZE*3 },
        LINE_WIDTH,
        RAYWHITE);

    DrawText("SCORE:", ORIGIN_X+(BOARD_W*CELL_SIZE)+BOX_SPACING+TEXT_PADDING, ORIGIN_Y-(int)LINE_WIDTH+TEXT_PADDING, TEXT_SIZE, RAYWHITE);
    DrawText(TextFormat("%010d", gameState.score), ORIGIN_X+(BOARD_W*CELL_SIZE)+BOX_SPACING+TEXT_PADDING, ORIGIN_Y-(int)LINE_WIDTH+TEXT_SIZE+(TEXT_PADDING*2), TEXT_SIZE*2, RAYWHITE);

    DrawRectangleLinesEx(
    (Rectangle){ ORIGIN_X+(BOARD_W*CELL_SIZE)+BOX_SPACING,
                 ORIGIN_Y-(int)LINE_WIDTH+(CELL_SIZE*3)+BOX_SPACING,
                 (CELL_SIZE*5)+TEXT_PADDING,
		 (CELL_SIZE*6)+TEXT_PADDING },
        LINE_WIDTH,
        RAYWHITE);

    //------------------------NEXT PIECE PREVIEW------------------------
    DrawText("NEXT PIECE:", ORIGIN_X+(BOARD_W*CELL_SIZE)+BOX_SPACING+TEXT_PADDING, ORIGIN_Y-(int)LINE_WIDTH+(CELL_SIZE*3)+BOX_SPACING+TEXT_PADDING, TEXT_SIZE, RAYWHITE);
    
    uint16_t pieceRotation = PIECE_DEFS[gameState.nextPiece.type].rotations[gameState.nextPiece.rotation];
    for (int i=0; i<4; i++) {
        for (int j=0; j<4; j++) {
            if (GetPieceCell(pieceRotation, i, j)) {
                DrawTexture(cell,
		            ORIGIN_X+(BOARD_W*CELL_SIZE)+BOX_SPACING+TEXT_PADDING+(j*CELL_SIZE),
		            ORIGIN_Y-(int)LINE_WIDTH+(CELL_SIZE*3)+BOX_SPACING+TEXT_SIZE+(TEXT_PADDING*2)+(i*CELL_SIZE),
			    PIECE_DEFS[gameState.nextPiece.type].color);
            }
        }
    }

    //------------------------------LEVEL------------------------------
    DrawRectangleLinesEx(
    (Rectangle){ ORIGIN_X+(BOARD_W*CELL_SIZE)+BOX_SPACING,
                 ORIGIN_Y-(int)LINE_WIDTH+(CELL_SIZE*9)+(BOX_SPACING*2)+TEXT_PADDING,
                 (CELL_SIZE*5)+TEXT_PADDING,
		 CELL_SIZE*3 },
        LINE_WIDTH,
        RAYWHITE);

    DrawText("LEVEL:", ORIGIN_X+(BOARD_W*CELL_SIZE)+BOX_SPACING+TEXT_PADDING, ORIGIN_Y-(int)LINE_WIDTH+(CELL_SIZE*9)+(BOX_SPACING*2)+(TEXT_PADDING*2), TEXT_SIZE, RAYWHITE);
    DrawText(TextFormat("%d", gameState.level), ORIGIN_X+(BOARD_W*CELL_SIZE)+BOX_SPACING+TEXT_PADDING, ORIGIN_Y-(int)LINE_WIDTH+(CELL_SIZE*9)+(BOX_SPACING*2)+TEXT_SIZE+(TEXT_PADDING*3), TEXT_SIZE*2, RAYWHITE);
}

// Maybe DrawMenu() and DrawGameOver() could be made into one, but since their text and size for titles differs it was left as two.
// This could be passed as an argument, but could be confusing from the caller file's perspective and would make it so there's more than one place where the menus need to be modified.
int DrawGameOver(GameState *gameState, ImageButton *menuButtons, const size_t buttonCount) {
    int buttonPressed = 0;

    UpdateMenuSelection(gameState, menuButtons, buttonCount);
    
    ClearBackground(BLACK);

    // Gameover screen consists of text and buttons.
    DrawText("GAME OVER",
             (SCREEN_WIDTH/2) - (MeasureText("GAME OVER", TITLE_SIZE/2)/2),
             TITLE_PADDING,
             TITLE_SIZE/2,
             RAYWHITE
             );

    for (size_t i=0; i<buttonCount; i++) {
        bool highlighted = (gameState->selectedButton == i);
        if (UpdateImageButton(&menuButtons[i], highlighted)) {
            buttonPressed = menuButtons[i].id;
        }
    }
    
    // The function returns the corresponding id of the button pressed, if any.
    return buttonPressed;
}

