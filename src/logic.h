#ifndef LOGIC_H
#define LOGIC_H

/**
 * raylib.h (rendering code) shouldn't be included here
 */

#include "game.h"
#include "types.h"

void printv2(Vector2 v);

f32 clampf(f32 val, f32 mn, f32 mx);

/**
 * mutates ship in memory
 */
Vector2 calcShipPosition(State* s, f32 frameTime, bool isLeft, f32 minPos, f32 maxPos);

/**
 * mutates the alien positions in memory
 */
void calcAliensPositions(State* state, f32 frameTime);

#endif // LOGIC_H
