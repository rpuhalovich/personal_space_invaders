#ifndef LOGIC_H
#define LOGIC_H

/**
 * raylib rendering code shouldn't be included here
 */

#include "game.h"
#include "types.h"

void printv2(Vector2 v);

f32 clampf(f32 val, f32 mn, f32 mx);

void randInit();

/**
 * returns random int between 0 and max INCLUSIVE
 */
i32 randInt(i32 max);

/**
 * mutates ship in memory
 */
void calcShipPosition(State* s, f32 frameTime, bool isLeft, f32 minPos, f32 maxPos);

/**
 * tick functions mutate state before rendering
 */
void tickGame(State* state, f32 time, f32 frameTime);
void tickMenu(State* state, f32 time, f32 frameTime);

#endif // LOGIC_H
