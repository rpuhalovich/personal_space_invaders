#ifndef BARRIER_H
#define BARRIER_H

#include "types.h"

typedef enum {
    TOP_LEFT = 0,
    BOTTOM_LEFT,
    TOP_RIGHT,
    BOTTOM_RIGHT,
    FULL,
} BarrierSprite;

typedef struct {
    u8 health;
    BarrierSprite sprite;
} BarrierBlock;

#define BARRIER_ROWS 3
#define BARRIER_COLS 4

// BarrierBlock defaultBarrierBlocks[BARRIER_ROWS][BARRIER_COLS] = {
//     {0, 0, 0, 0},
//     {0, 0, 0, 0},
//     {0, 0, 0, 0},
// };

typedef struct {
    BarrierBlock barrierBlocks[BARRIER_ROWS][BARRIER_COLS];
} Barrier;

#endif // BARRIER_H
