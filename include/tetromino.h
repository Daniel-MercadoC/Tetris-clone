#ifndef TETROMINO_H
#define TETROMINO_H

#include <stdint.h>

// These macros are used so it's easier to visualize piece representations with bits (e.g. 0010 means there's a block on column 3 in the current row).
// There's a simpler way to represent this using binary literals, which would look like: rotation[x] = 0b0010001000100010 for an upstanding I piece.
// It's not as explicit as how it looks using the macro version, though, and it's only supported officially as of C23, thus limiting the programmer in context to build in.
#define ROW4(a,b,c,d) ((a)<<3 | (b)<<2 | (c)<<1 | (d))
#define GRID(r0,r1,r2,r3) ((uint16_t)((r0)<<12 | (r1)<<8 | (r2)<<4 | (r3)))

typedef enum {
    PIECE_I,
    PIECE_J,
    PIECE_L,
    PIECE_O,
    PIECE_S,
    PIECE_T,
    PIECE_Z,
    PIECE_COUNT
} PieceType;

typedef struct {
    uint16_t rotations[4];
    Color color;
} PieceDef;

// Definition on all rotations, to prevent the need to calculate logically every single rotation whenever it happens; which would also make it trickier to
// implement functionality like t-spins
//
// IMPORTANT: Never fill any rotation such that the first row has the only cell in that column. i.e.:
//
// 1 1 0 0 <- WRONG
// 0 1 0 0
// 0 1 0 0
// 0 0 0 0
//
// This is because of the implementation of bottom collision checking, which assumes this won't ever happen, and allows initialization of all values to be 0.
// If this ever changes, the table of bottomCells would need to be initialized to -1 or add a sentinel value.
static const PieceDef PIECE_DEFS[PIECE_COUNT] = {
    [PIECE_I] = {
        .rotations = {
            // Bit layout of a rotation value (row0 = ROW4 written LAST, row3 = ROW4 written FIRST):
            //
            //   col:  3 | 2 | 1 | 0
            //        ---|---|---|---
            //   row0: d | c | b | a   <- last ROW4(a,b,c,d) argument
            //   row1: d | c | b | a
            //   row2: d | c | b | a
            //   row3: d | c | b | a   <- first ROW4(a,b,c,d) argument (bits 12-15)
            //
            // This follows raylib's convention of x=0 being the left and y=0 being the top, which makes an I piece like so:
            //
            //    0 0 0 0    0 1 0 0
            //    0 0 0 0    0 1 0 0
            //    1 1 1 1    0 1 0 0
            //    0 0 0 0    0 1 0 0
            //
            GRID(ROW4(0,0,0,0), ROW4(1,1,1,1), ROW4(0,0,0,0), ROW4(0,0,0,0)),
            GRID(ROW4(0,0,1,0), ROW4(0,0,1,0), ROW4(0,0,1,0), ROW4(0,0,1,0)),
            GRID(ROW4(0,0,0,0), ROW4(1,1,1,1), ROW4(0,0,0,0), ROW4(0,0,0,0)),
            GRID(ROW4(0,0,1,0), ROW4(0,0,1,0), ROW4(0,0,1,0), ROW4(0,0,1,0))
        },
        // Raylib SKYBLUE. The CLITERAL can't be used, since cl compilation detects it as non-constant
        .color = { 102, 191, 255, 255 }
    },
    [PIECE_J] = {
        .rotations = {
            //
            //    0 0 0 0    0 0 0 0    0 0 0 0    0 0 0 0
            //    0 0 0 0    0 1 0 0    1 0 0 0    0 1 1 0
            //    1 1 1 0    0 1 0 0    1 1 1 0    0 1 0 0
            //    0 0 1 0    1 1 0 0    0 0 0 0    0 1 0 0
            //
            GRID(ROW4(0,1,0,0), ROW4(0,1,1,1), ROW4(0,0,0,0), ROW4(0,0,0,0)),
            GRID(ROW4(0,0,1,1), ROW4(0,0,1,0), ROW4(0,0,1,0), ROW4(0,0,0,0)),
            GRID(ROW4(0,0,0,0), ROW4(0,1,1,1), ROW4(0,0,0,1), ROW4(0,0,0,0)),
            GRID(ROW4(0,0,1,0), ROW4(0,0,1,0), ROW4(0,1,1,0), ROW4(0,0,0,0))
        },
        // Raylib BLUE
        .color = { 0, 121, 241, 255 }
    },
    [PIECE_L] = {
        .rotations = {
            //
            //    0 0 0 0    0 0 0 0    0 0 0 0    0 0 0 0
            //    0 0 1 0    0 1 0 0    0 0 0 0    1 1 0 0
            //    1 1 1 0    0 1 0 0    1 1 1 0    0 1 0 0
            //    0 0 0 0    0 1 1 0    1 0 0 0    0 1 0 0
            //
            GRID(ROW4(0,0,0,0), ROW4(0,1,1,1), ROW4(0,1,0,0), ROW4(0,0,0,0)),
            GRID(ROW4(0,1,1,0), ROW4(0,0,1,0), ROW4(0,0,1,0), ROW4(0,0,0,0)),
            GRID(ROW4(0,0,0,1), ROW4(0,1,1,1), ROW4(0,0,0,0), ROW4(0,0,0,0)),
            GRID(ROW4(0,0,1,0), ROW4(0,0,1,0), ROW4(0,0,1,1), ROW4(0,0,0,0))
        },
        // Raylib ORANGE
        .color = { 255, 161, 0, 255 }
    },
    [PIECE_O] = {
        .rotations = {
            //
            //    0 0 0 0    0 0 0 0    0 0 0 0    0 0 0 0
            //    0 1 1 0    0 1 1 0    0 1 1 0    0 1 1 0
            //    0 1 1 0    0 1 1 0    0 1 1 0    0 1 1 0
            //    0 0 0 0    0 0 0 0    0 0 0 0    0 0 0 0
            //
            GRID(ROW4(0,0,0,0), ROW4(0,1,1,0), ROW4(0,1,1,0), ROW4(0,0,0,0)),
            GRID(ROW4(0,0,0,0), ROW4(0,1,1,0), ROW4(0,1,1,0), ROW4(0,0,0,0)),
            GRID(ROW4(0,0,0,0), ROW4(0,1,1,0), ROW4(0,1,1,0), ROW4(0,0,0,0)),
            GRID(ROW4(0,0,0,0), ROW4(0,1,1,0), ROW4(0,1,1,0), ROW4(0,0,0,0))
        },
        // Raylib YELLOW
        .color = { 253, 249, 0, 255 }
    },
    [PIECE_S] = {
        .rotations = {
            //
            //    0 0 0 0    0 1 0 0    0 0 0 0    0 1 0 0
            //    0 1 1 0    0 1 1 0    0 1 1 0    0 1 1 0
            //    1 1 0 0    0 0 1 0    1 1 0 0    0 0 1 0
            //    0 0 0 0    0 0 0 0    0 0 0 0    0 0 0 0
            //
            GRID(ROW4(0,0,0,0), ROW4(0,0,1,1), ROW4(0,1,1,0), ROW4(0,0,0,0)),
            GRID(ROW4(0,0,0,0), ROW4(0,1,0,0), ROW4(0,1,1,0), ROW4(0,0,1,0)),
            GRID(ROW4(0,0,0,0), ROW4(0,0,1,1), ROW4(0,1,1,0), ROW4(0,0,0,0)),
            GRID(ROW4(0,0,0,0), ROW4(0,1,0,0), ROW4(0,1,1,0), ROW4(0,0,1,0))
        },
        // Raylib LIME
        .color = { 0, 158, 47, 255 }
    },
    [PIECE_T] = {
        .rotations = {
            //
            //    0 0 0 0    0 0 0 0    0 0 0 0    0 0 0 0
            //    0 1 0 0    0 1 0 0    0 0 0 0    0 1 0 0
            //    1 1 1 0    0 1 1 0    1 1 1 0    1 1 0 0
            //    0 0 0 0    0 1 0 0    0 1 0 0    0 1 0 0
            //
            GRID(ROW4(0,0,0,0), ROW4(0,1,1,1), ROW4(0,0,1,0), ROW4(0,0,0,0)),
            GRID(ROW4(0,0,1,0), ROW4(0,1,1,0), ROW4(0,0,1,0), ROW4(0,0,0,0)),
            GRID(ROW4(0,0,1,0), ROW4(0,1,1,1), ROW4(0,0,0,0), ROW4(0,0,0,0)),
            GRID(ROW4(0,0,1,0), ROW4(0,0,1,1), ROW4(0,0,1,0), ROW4(0,0,0,0))
        },
        // Raylib VIOLET
        .color = { 135, 60, 190, 255 }
    },
    [PIECE_Z] = {
        .rotations = {
            //
            //    0 0 0 0    0 0 1 0    0 0 0 0    0 0 1 0
            //    0 1 1 0    0 1 1 0    0 1 1 0    0 1 1 0
            //    0 0 1 1    0 1 0 0    0 0 1 1    0 1 0 0
            //    0 0 0 0    0 0 0 0    0 0 0 0    0 0 0 0
            //
            GRID(ROW4(0,0,0,0), ROW4(1,1,0,0), ROW4(0,1,1,0), ROW4(0,0,0,0)),
            GRID(ROW4(0,0,0,0), ROW4(0,0,1,0), ROW4(0,1,1,0), ROW4(0,1,0,0)),
            GRID(ROW4(0,0,0,0), ROW4(1,1,0,0), ROW4(0,1,1,0), ROW4(0,0,0,0)),
            GRID(ROW4(0,0,0,0), ROW4(0,0,1,0), ROW4(0,1,1,0), ROW4(0,1,0,0))
        },
        // Raylib RED
        .color = { 230, 41, 55, 255 }
    }
};

typedef struct { int lowestRow[4][4]; } BottomCells;

#define NUM_KICKS 9

typedef struct { int dx, dy; } KickOffset;
static const KickOffset KICK_OFFSETS[NUM_KICKS] = {
    { 0, 0},
    {-1, 0},
    { 1, 0},
    { 0,-1},
    { 0, 1},
    {-1,-1},
    { 1,-1},
    {-1, 1},
    { 1, 1}
};

// Piece types are alphabetically sorted everywhere, so type0 = I piece and type1 = J piece.
typedef struct {
    int type;
    int rotation;
    int x, y;
} ActivePiece;

static inline int GetPieceCell(uint16_t rotation, int row, int col) {
    return (rotation >> ((row * 4) + col) & 1);
}
void TryRotateInDirection(ActivePiece *activePiece, bool isLeft);
void GenerateBottomCollisionTable(BottomCells *bottomCells);

#endif
