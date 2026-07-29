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
static const PieceDef PIECE_DEFS[1] = {
    [PIECE_I] = {
        .rotations = {
            GRID(ROW4(0,0,0,0), ROW4(1,1,1,1), ROW4(0,0,0,0), ROW4(0,0,0,0)),
            GRID(ROW4(0,0,1,0), ROW4(0,0,1,0), ROW4(0,0,1,0), ROW4(0,0,1,0)),
            GRID(ROW4(0,0,0,0), ROW4(1,1,1,1), ROW4(0,0,0,0), ROW4(0,0,0,0)),
            GRID(ROW4(0,0,1,0), ROW4(0,0,1,0), ROW4(0,0,1,0), ROW4(0,0,1,0))
        },
        // Raylib SKYBLUE. The CLITERAL can't be used, since cl compilation detects it as non-constant
        .color = { 102, 191, 255, 255 }
    }
};

// Piece types are alphabetically sorted everywhere, so type0 = I piece and type1 = J piece.
typedef struct {
    int type;
    int rotation;
    int x, y;
} ActivePiece;

void InitPieceDefs();

#endif
