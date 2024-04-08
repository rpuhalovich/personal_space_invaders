#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "game.h"
#include "logic.h"

void pri32v2(Vector2 v) {
    printf("x: %f, y: %f\n", v.x, v.y);
}

f32 clampf(f32 val, f32 mn, f32 mx) {
    return fmax(mn, fmin(val, mx));
}

void randInit() {
    srand(time(NULL));
}

i32 randInt(i32 max) {
    return rand() % max;
}

// TODO: this shouldn't be a function
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
    s->ship.pos = resPos;
}

void tickGame(State* state, f32 time, f32 frameTime) {
    // bullet
    {
        if (state->ship.bullet.start.y <= 0.f) state->ship.bullet.isFired = false;
        f32 m = state->ship.bullet.speed * frameTime;
        state->ship.bullet.start.y -= m;
        state->ship.bullet.end.y -= m;

        for (i32 r = 0; r < ALIEN_ROWS; r++) {
            for (i32 c = 0; c < ALIEN_COLS; c++) {
                Alien a = state->aliens.alienGrid[r][c];
                if (a.isDed) continue;
                Rectangle rec = {.x = a.pos.x, .y = a.pos.y, .width = a.texture.width, .height = a.texture.height};
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
        // shoot and update bullets
        if (time - state->aliens.lastShotTime >= state->aliens.shootTime) {
            printf("FIRE!\n");
            for (i32 i = 0; i < NUM_ALIEN_BULLETS; i++) {
                Bullet* curBullet = &state->aliens.bullets[i];
                if (!curBullet->isFired) {
                    Alien* randAlien = &state->aliens.alienGrid[randInt(ALIEN_ROWS)][randInt(ALIEN_COLS)];
                    Bullet b = { .speed = 1.f, .width = 1.f, .height = 10.f, .isFired = true };
                    Vector2 bulletStartPos = {randAlien->pos.x + (float)randAlien->texture.width / 2, randAlien->pos.y + randAlien->texture.height};
                    Vector2 bulletEndPos = bulletStartPos;
                    bulletEndPos.y = bulletEndPos.y + b.height;
                    b.start = bulletStartPos;
                    b.end = bulletEndPos;
                    state->aliens.lastShotTime = time;
                    state->aliens.bullets[i] = b;
                    break;
                }
            }
        }

        for (i32 i = 0; i < NUM_ALIEN_BULLETS; i++) {
            Bullet* curBullet = &state->aliens.bullets[i];
            curBullet->end.y += curBullet->speed;
            curBullet->start.y += curBullet->speed;
        }

        for (i32 i = 0; i < NUM_ALIEN_BULLETS; i++) {
            Bullet* curBullet = &state->aliens.bullets[i];
            if (curBullet->start.y >= state->window.height) curBullet->isFired = false;
        }

        // movement
        i32 dir = state->aliens.isLeft ? -1 : 1;
        f32 offset = dir * abs(state->aliens.speed) * GetFrameTime();
        for (i32 r = 0; r < ALIEN_ROWS; r++) {
            for (i32 c = 0; c < ALIEN_COLS; c++) {
                state->aliens.alienGrid[r][c].pos.x += offset;
            }
        }

        f32 lmargin = state->window.margin;
        f32 rmargin = state->window.width - state->window.margin;

        bool moveDown = false;
        for (i32 r = 0; r < ALIEN_ROWS; r++) {
            for (i32 c = 0; c < ALIEN_COLS; c++) {
                Alien a = state->aliens.alienGrid[r][c];
                f32 textureWidth = state->aliens.alienGrid[r][c].texture.width;
                if (a.pos.x < lmargin || (a.pos.x + textureWidth) > rmargin && !a.isDed) {
                    state->aliens.isLeft = !state->aliens.isLeft;
                    moveDown = true;
                }
            }
        }

        if (moveDown) {
            for (i32 r = 0; r < ALIEN_ROWS; r++) {
                for (i32 c = 0; c < ALIEN_COLS; c++) {
                    state->aliens.alienGrid[r][c].pos.y += state->aliens.alienGrid[r][c].texture.height;
                    f32 x = state->aliens.alienGrid[r][c].pos.x;
                    state->aliens.alienGrid[r][c].pos.x = clampf(x, lmargin + 1.f, rmargin - 1.f);
                }
            }
        }
    }
}

void tickMenu(State* state, f32 time, f32 frameTime) {
    // TODO: menu logic
}
