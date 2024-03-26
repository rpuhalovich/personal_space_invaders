#include <stdio.h>

#include "logic.h"
#include "raymath.h"

void printv2(Vector2 v) {
    printf("x: %f, y: %f\n", v.x, v.y);
}

f32 clampf(f32 val, f32 mn, f32 mx) {
    return fmax(mn, fmin(val, mx));
}

Vector2 calcShipPos(Vector2 shipPos, f32 shipSpeed, bool isLeft, f32 frameTime, f32 minPos, f32 maxPos) {
    f32 m = shipSpeed * frameTime;
    Vector2 resPos = shipPos;
    if (isLeft) {
        resPos.x -= m;
        resPos.x = fmaxf(minPos, resPos.x);
    } else {
        resPos.x += m;
        resPos.x = fminf(maxPos, resPos.x);
    }
    return resPos;
}
