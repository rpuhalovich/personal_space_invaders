#ifndef LOGIC_H
#define LOGIC_H

/**
 * raylib.h (rendering code) shouldn't be included here
 */

#include <raymath.h>

#include "types.h"

void printv2(Vector2 v);

f32 clampf(f32 val, f32 mn, f32 mx);

Vector2 calcShipPos(Vector2 shipPos, f32 shipSpeed, bool isLeft, f32 frameTime, f32 minPos, f32 maxPos);

#endif // LOGIC_H
