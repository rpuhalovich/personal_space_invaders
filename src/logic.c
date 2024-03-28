#include <stdio.h>

#include "logic.h"
#include "game.h"

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

void tick(State* state, f32 frameTime) {
    // bullet
    {
        if (state->ship.bullet.start.y <= 0.f) state->ship.bullet.isFired = false;
        f32 m = state->ship.bullet.speed * frameTime;
        state->ship.bullet.start.y -= m;
        state->ship.bullet.end.y -= m;

        for (int r = 0; r < ALIEN_ROWS; r++) {
            for (int c = 0; c < ALIEN_COLS; c++) {
                Alien a = state->aliens.alienGrid[r][c];
                if (a.isDed) continue;
                Rectangle rec = { .x = a.pos.x, .y = a.pos.y, .width = a.texture.width, .height = a.texture.height };
                if (CheckCollisionPointRec(state->ship.bullet.start, rec) && state->ship.bullet.isFired) {
                    state->aliens.alienGrid[r][c].isDed = true;
                    state->ship.bullet.isFired = false;
                    state->score += state->aliens.killScore;
                }
            }
        }
    }

    // aliens
    if (!state->aliens.isPaused) {
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
                Alien a = state->aliens.alienGrid[r][c];
                f32 textureWidth = state->aliens.alienGrid[r][c].texture.width;
                if (a.pos.x < state->window.leftMargin || (a.pos.x + textureWidth) > state->window.rightMargin && !a.isDed) {
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
}

void shoot(State* s) {
    if (s->ship.bullet.isFired) return;
    Vector2 pos = {s->ship.pos.x + (f32)s->ship.texture.width / 2, s->ship.pos.y};
    s->ship.bullet.end = pos;

    s->ship.bullet.start = pos;
    s->ship.bullet.start.y -= s->ship.bullet.height;

    s->ship.bullet.isFired = true;
}
