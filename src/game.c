#include <stdlib.h>

#include "game.h"
#include "logic.h"
#include "raylib.h"
#include "raymath.h"

State state = {0};

void init(void) {
    State s = {
        .time = 0,

        .window.margin = 10.f,
        .window.width = 224,
        .window.height = 256,
        .window.scaleFactor = SCALE_FACTOR,
        .window.targetFps = 60,

        .ship.speed = 100.f,

        .aliens.alienRows = ALIEN_ROWS,
        .aliens.alienCols = ALIEN_COLS,
        .aliens.alienGrid = {0},
        .aliens.spacing = 5.f,
        .aliens.speed = 20.f,
        .aliens.isLeft = false,
    };

    s.window.leftMargin = s.window.margin;
    s.window.rightMargin = s.window.width - (s.ship.texture.width + s.window.margin);
    state = s;

    InitWindow(s.window.width * SCALE_FACTOR, s.window.height * SCALE_FACTOR, "Personal Space Invaders");
    SetTargetFPS(state.window.targetFps);

    // ship
    {
        Image img = LoadImage("./res/ship.png");
        state.ship.texture = LoadTextureFromImage(img);
        UnloadImage(img);
        Vector2 pos = {state.window.margin, state.window.height - (state.ship.texture.height + state.window.margin)};
        state.ship.pos = pos;
    }

    // aliens
    {
        Image img = LoadImage("./res/alien.png");
        Texture texture = LoadTextureFromImage(img);
        UnloadImage(img);

        for (int r = 0; r < ALIEN_ROWS; r++) {
            for (int c = 0; c < ALIEN_COLS; c++) {
                Vector2 pos = {(state.aliens.spacing + texture.width) * c, (state.aliens.spacing + texture.height) * r};
                pos.x += state.window.margin;
                pos.y += state.window.margin;
                Alien a = {texture, pos};
                state.aliens.alienGrid[r][c] = a;
            }
        }
    }
}

void update(void) {
    // input
    {
        f32 minShipPos = state.window.margin;
        f32 maxShipPos = state.window.width - state.ship.texture.width - state.window.margin;
        if (IsKeyDown(KEY_LEFT)) calcShipPosition(&state, GetFrameTime(), true, minShipPos, maxShipPos);
        if (IsKeyDown(KEY_RIGHT)) calcShipPosition(&state, GetFrameTime(), false, minShipPos, maxShipPos);
    }

    // logic
    {
        calcAliensPositions(&state, GetFrameTime());
    }

    // render
    {
        ClearBackground(BLACK);

        // draw ship
        DrawTextureEx(state.ship.texture, Vector2Scale(state.ship.pos, state.window.scaleFactor), 0, state.window.scaleFactor, WHITE);

        // draw aliens
        for (int r = 0; r < ALIEN_ROWS; r++) {
            for (int c = 0; c < ALIEN_COLS; c++) {
                Vector2 alienPos = Vector2Scale(state.aliens.alienGrid[r][c].pos, state.window.scaleFactor);
                DrawTextureEx(state.aliens.alienGrid[r][c].texture, alienPos, 0, state.window.scaleFactor, WHITE);
            }
        }
    }
}

void cleanup() {
    UnloadTexture(state.ship.texture);
    UnloadTexture(state.aliens.alienGrid[0][0].texture);
}
