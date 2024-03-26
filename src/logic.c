#include <stdio.h>

#include "logic.h"

void printv2(Vector2 v) {
    printf("x: %f, y: %f\n", v.x, v.y);
}

f32 clampf(f32 val, f32 mn, f32 mx) {
    return fmax(mn, fmin(val, mx));
}

void calcShipPosition(State* s, f32 frameTime, bool isLeft, f32 minPos, f32 maxPos) {
    f32 m = s->ship.speed * frameTime;
    Vector2 resPos = s->ship.pos;
    if (isLeft) {
        resPos.x -= m;
        resPos.x = fmaxf(minPos, resPos.x);
    } else {
        resPos.x += m;
        resPos.x = fminf(maxPos, resPos.x);
    }
    s->ship.pos =resPos;
}

void calcAliensPositions(State* state, f32 frameTime) {
    i32 dir = state->aliens.isLeft ? -1 : 1;
    f32 offset = dir * abs(state->aliens.speed) * GetFrameTime();
    for (int r = 0; r < ALIEN_ROWS; r++) {
        for (int c = 0; c < ALIEN_COLS; c++) {
            state->aliens.alienGrid[r][c].pos.x += offset;
        }
    }

    bool moveDown = false;
    for (int r = 0; r < ALIEN_ROWS; r++) {
        for (int c = 0; c < ALIEN_COLS; c++) {
            Vector2 pos = state->aliens.alienGrid[r][c].pos;
            f32 textureWidth = state->aliens.alienGrid[r][c].texture.width;
            if (pos.x < state->window.leftMargin || (pos.x + textureWidth) > state->window.rightMargin) {
                state->aliens.isLeft = !state->aliens.isLeft;
                moveDown = true;
            }
        }
    }

    if (moveDown) {
        for (int r = 0; r < ALIEN_ROWS; r++) {
            for (int c = 0; c < ALIEN_COLS; c++) {
                state->aliens.alienGrid[r][c].pos.y += state->aliens.alienGrid[r][c].texture.height;
                f32 x = state->aliens.alienGrid[r][c].pos.x;
                f32 lmargin = state->window.leftMargin + 1.f;
                f32 rmargin = state->window.rightMargin - 1.f;
                state->aliens.alienGrid[r][c].pos.x = clampf(x, lmargin, rmargin);
            }
        }
    }
}
