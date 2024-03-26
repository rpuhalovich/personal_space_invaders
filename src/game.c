#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "game.h"
#include "logic.h"
#include "raylib.h"
#include "raymath.h"

State state = {0};

void init(void) {
    State s = {
        .time = 0,

        .window.leftRightMargin = 10.f,
        .window.width = 224,
        .window.height = 256,
        .window.scaleFactor = SCALE_FACTOR,
        .window.targetFps = 60,

        .ship.speed = 100.f,

        .aliens.alienGrid = {0},
        .aliens.speed = 20.f,
        .aliens.isLeft = false,
    };
    state = s;

    InitWindow(s.window.width * SCALE_FACTOR, s.window.height * SCALE_FACTOR, "Personal Space Invaders");
    SetTargetFPS(state.window.targetFps);

    // ship
    {
        Image img = LoadImage("./res/ship.png");
        state.ship.texture = LoadTextureFromImage(img);
        UnloadImage(img);
        Vector2 pos = {state.window.leftRightMargin, state.window.height - (state.ship.texture.height + state.window.leftRightMargin)};
        state.ship.pos = pos;
    }

    // aliens
    {
        Image img = LoadImage("./res/alien.png");
        Texture texture = LoadTextureFromImage(img);
        UnloadImage(img);

        for (int r = 0; r < ALIEN_ROWS; r++) {
            for (int c = 0; c < ALIEN_COLS; c++) {
                Vector2 pos = {0, 0};
                Alien a = {texture, pos};
                state.aliens.alienGrid[r][c] = a;
            }
        }
    }
}

void update(void) {
    // input
    {
        f32 minShipPos = state.window.leftRightMargin;
        f32 maxShipPos = state.window.width - state.ship.texture.width - state.window.leftRightMargin;
        if (IsKeyDown(KEY_LEFT)) {
            state.ship.pos = calcShipPos(state.ship.pos, state.ship.speed, true, GetFrameTime(), minShipPos, maxShipPos);
        }
        if (IsKeyDown(KEY_RIGHT)) {
            state.ship.pos = calcShipPos(state.ship.pos, state.ship.speed, false, GetFrameTime(), minShipPos, maxShipPos);
        }
    }

#if 0
    // logic
    {
        // aliens
        {
            f32 dir = state.aliens.isLeft ? -1 : 1;
            state.aliens.speed = abs(state.aliens.speed) * dir;
            state.aliens.alienOffset += state.aliens.speed * GetFrameTime();

            f32 spacing = 5.f, offset = 21.f;
            for (int r = 0; r < ALIEN_ROWS; r++) {
                for (int c = 0; c < ALIEN_COLS; c++) {
                    Alien a = state.aliens.alienGrid[r][c];
                    Vector2 newpos = {(a.texture.width + spacing) * c,
                                      (a.texture.height + spacing) * r};
                    newpos.y += offset * 3;
                    newpos.y += state.aliens.yOffset;
                    newpos.x += offset;
                    newpos.x += state.aliens.alienOffset;
                    state.aliens.alienGrid[r][c].pos = newpos;
                }
            }

            for (int r = 0; r < ALIEN_ROWS; r++) {
                for (int c = 0; c < ALIEN_COLS; c++) {
                    Vector2 pos = state.aliens.alienGrid[r][c].pos;
                    if (pos.x > state.window.width - state.ship.texture.width -
                                    20.f ||
                        pos.x <= 20.f) {
                        state.aliens.isLeft = !state.aliens.isLeft;
                        state.aliens.yOffset +=
                            state.aliens.alienGrid[r][c].texture.height;
                        goto alien_continue;
                    }

                    f32 newx = clampf(pos.x,
                                      20.f + 1,
                                      state.window.width -
                                          state.ship.texture.width - 20.f + 1);
                    state.aliens.alienGrid[r][c].pos.x = newx;
                }
            }

        alien_continue:
            return;
        }
    }
#endif

    // render
    {
        ClearBackground(BLACK);

        // draw ship
        Vector2 shipPos = Vector2Scale(state.ship.pos, state.window.scaleFactor);
        DrawTextureEx(state.ship.texture, Vector2Scale(state.ship.pos, state.window.scaleFactor), 0, state.window.scaleFactor, WHITE);

        // draw aliens
        for (int r = 0; r < ALIEN_ROWS; r++) {
            for (int c = 0; c < ALIEN_COLS; c++) {
                Alien a = state.aliens.alienGrid[r][c];
                Vector2 alienPos = Vector2Scale(a.pos, state.window.scaleFactor);
                DrawTextureEx(a.texture, alienPos, 0, state.window.scaleFactor, WHITE);
            }
        }
    }
}

void cleanup() {
    UnloadTexture(state.ship.texture);
    UnloadTexture(state.aliens.alienGrid[0][0].texture);
}
