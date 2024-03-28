#include "game.h"
#include "logic.h"
#include "raylib.h"
#include "raymath.h"

State state = {0};

void init(void) {
    State s = {
        .score = 0,
        .time = 0,

        .window.fontSize = 10.f,
        .window.height = 256,
        .window.margin = 5.f,
        .window.scaleFactor = SCALE_FACTOR,
        .window.targetFps = 60,
        .window.width = 224,

        .ship.speed = 100.f,
        .ship.lives = 3,
        .ship.bullet.end = {0},
        .ship.bullet.height = 10.f,
        .ship.bullet.isFired = false,
        .ship.bullet.speed = 300.f,
        .ship.bullet.start = {0},
        .ship.bullet.width = 1.f,

        .aliens.alienCols = ALIEN_COLS,
        .aliens.alienGrid = {0},
        .aliens.alienRows = ALIEN_ROWS,
        .aliens.isLeft = false,
        .aliens.isPaused = false,
        .aliens.killScore = 100,
        .aliens.spacing = 5.f,
        .aliens.speed = 20.f,
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
        s.window.deathFloor = pos.y + 10.f;
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
                pos.y += state.window.margin + 10.f;
                Alien a = {texture, pos};
                state.aliens.alienGrid[r][c] = a;
            }
        }
    }
}

void update(void) {
    // handle input
    {
        f32 minShipPos = state.window.margin;
        f32 maxShipPos = state.window.width - state.ship.texture.width - state.window.margin;
        if (IsKeyDown(KEY_LEFT)) calcShipPosition(&state, GetFrameTime(), true, minShipPos, maxShipPos);
        if (IsKeyDown(KEY_RIGHT)) calcShipPosition(&state, GetFrameTime(), false, minShipPos, maxShipPos);
        if (IsKeyPressed(KEY_SPACE)) shoot(&state);
#ifdef PSI_DEBUG
        if (IsKeyPressed(KEY_P)) state.aliens.isPaused = !state.aliens.isPaused;
#endif
    }

    // logic that updates state
    tick(&state, GetFrameTime());

    // render state
    {
        ClearBackground(BLACK);

        // draw score
        char score[16];
        snprintf(score, sizeof(score), "%d", state.score);
        f32 fontSize = state.window.fontSize;
        f32 textWidth = MeasureText(score, fontSize * state.window.scaleFactor);
        DrawText(score, (state.window.width * state.window.scaleFactor / 2 - textWidth / 2), 5, fontSize * state.window.scaleFactor, WHITE);

        // draw ship
        DrawTextureEx(state.ship.texture, Vector2Scale(state.ship.pos, state.window.scaleFactor), 0, state.window.scaleFactor, WHITE);

        // draw aliens
        for (int r = 0; r < ALIEN_ROWS; r++) {
            for (int c = 0; c < ALIEN_COLS; c++) {
                if (state.aliens.alienGrid[r][c].isDed) continue;
                Vector2 alienPos = Vector2Scale(state.aliens.alienGrid[r][c].pos, state.window.scaleFactor);
                DrawTextureEx(state.aliens.alienGrid[r][c].texture, alienPos, 0, state.window.scaleFactor, WHITE);
            }
        }

        // draw bullet
        if (state.ship.bullet.isFired) {
            Vector2 start = Vector2Scale(state.ship.bullet.start, state.window.scaleFactor);
            Vector2 end = Vector2Scale(state.ship.bullet.end, state.window.scaleFactor);
            DrawLineEx(start, end, state.ship.bullet.width * state.window.scaleFactor, WHITE);
        }
    }
}

void cleanup() {
    UnloadTexture(state.ship.texture);
    UnloadTexture(state.aliens.alienGrid[0][0].texture);
}
