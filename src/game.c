#include "game.h"
#include "logic.h"
#include "raylib.h"
#include "raymath.h"

State state = {0};

void init(void) {
    randInit();

    State s = {
        .gameMode = GAME,
        .score = 0,
        .time = 0,

        .window.fontSize = 10.f,
        .window.width = 224,
        .window.height = 256,
        .window.margin = 5.f,
        .window.scaleFactor = SCALE_FACTOR,
        .window.targetFps = 60,

        .ship.speed = 100.f,
        .ship.lives = 5,
        .ship.bullet.end = {0},
        .ship.bullet.height = 10.f,
        .ship.bullet.isFired = false,
        .ship.bullet.speed = 300.f,
        .ship.bullet.start = {0},
        .ship.bullet.width = 1.f,

        .aliens.alienGrid = {0},
        .aliens.alienCols = ALIEN_COLS,
        .aliens.alienRows = ALIEN_ROWS,
        .aliens.isLeft = false,
        .aliens.isPaused = false,
        .aliens.killScore = 100,
        .aliens.spacing = 5.f,
        .aliens.speed = 20.f,
        .aliens.lastMoveTime = 0.f,
        .aliens.moveInterval = 20.f,
        .aliens.shootTime = 2.f,
    };

    state = s;

    InitWindow(s.window.width * SCALE_FACTOR, s.window.height * SCALE_FACTOR, "Personal Space Invaders");
    SetTargetFPS(state.window.targetFps);

    // ship
    {
        Image img = LoadImage(RES_DIR "ship.png");
        state.ship.texture = LoadTextureFromImage(img);
        UnloadImage(img);
        Vector2 pos = {state.window.margin, state.window.height - (state.ship.texture.height + state.window.margin)};
        state.ship.pos = pos;
        s.window.deathFloor = pos.y + 10.f;
    }

    // barriers
    {
        // TODO: make barriers
    }

    // aliens
    {
        Image img = LoadImage(RES_DIR "alien.png");
        Texture texture = LoadTextureFromImage(img);
        UnloadImage(img);

        for (i32 r = 0; r < ALIEN_ROWS; r++) {
            for (i32 c = 0; c < ALIEN_COLS; c++) {
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
    if (state.gameMode == GAME) {
        // handle input
        {
            f32 minShipPos = state.window.margin;
            f32 maxShipPos = state.window.width - state.ship.texture.width - state.window.margin;
            if (IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A)) calcShipPosition(&state, GetFrameTime(), true, minShipPos, maxShipPos);
            if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D)) calcShipPosition(&state, GetFrameTime(), false, minShipPos, maxShipPos);
            if (IsKeyPressed(KEY_SPACE) || IsKeyPressed(KEY_X)) {
                if (state.ship.bullet.isFired) return;
                Vector2 pos = {state.ship.pos.x + (f32)state.ship.texture.width / 2, state.ship.pos.y};
                state.ship.bullet.end = pos;
                state.ship.bullet.start = pos;
                state.ship.bullet.start.y -= state.ship.bullet.height;
                state.ship.bullet.isFired = true;
            }
            if (IsKeyPressed(KEY_M)) state.gameMode = MENU;

#ifdef PSI_DEBUG
            if (IsKeyPressed(KEY_P)) state.aliens.isPaused = !state.aliens.isPaused;
            if (IsKeyPressed(KEY_RIGHT_BRACKET)) state.ship.lives = MIN(5, state.ship.lives + 1);
            if (IsKeyPressed(KEY_LEFT_BRACKET)) state.ship.lives = MAX(0, state.ship.lives - 1);
#endif
        }

        // logic
        tickGame(&state, GetTime(), GetFrameTime());

        // render state
        {
            i32 sf = state.window.scaleFactor;

            ClearBackground(BLACK);

            // draw score
            char score[16];
            snprintf(score, sizeof(score), "%d", state.score);
            f32 fontSize = state.window.fontSize;
            f32 textWidth = MeasureText(score, fontSize);
            f32 textXpos = (state.window.width - state.window.margin - textWidth) * sf;
            DrawText(score, textXpos, 5, fontSize * sf, WHITE);

            // draw ship
            DrawTextureEx(state.ship.texture, Vector2Scale(state.ship.pos, sf), 0, sf, WHITE);

            // draw ship bullet
            if (state.ship.bullet.isFired) {
                Vector2 start = Vector2Scale(state.ship.bullet.start, sf);
                Vector2 end = Vector2Scale(state.ship.bullet.end, sf);
                DrawLineEx(start, end, state.ship.bullet.width * sf, WHITE);
            }

            // draw ship lives
            for (i32 i = 0; i < state.ship.lives; i++) {
                Vector2 pos = {state.window.margin + (state.ship.texture.width + state.window.margin) * i, state.window.margin};
                DrawTextureEx(state.ship.texture, Vector2Scale(pos, sf), 0, sf, WHITE);
            }

            // draw aliens
            for (i32 r = 0; r < ALIEN_ROWS; r++) {
                for (i32 c = 0; c < ALIEN_COLS; c++) {
                    if (state.aliens.alienGrid[r][c].isDed)
                        continue;
                    Vector2 alienPos = Vector2Scale(state.aliens.alienGrid[r][c].pos, sf);
                    DrawTextureEx(state.aliens.alienGrid[r][c].texture, alienPos, 0, sf, WHITE);
                }
            }

            // draw alien bullets
            for (i32 i = 0; i < NUM_ALIEN_BULLETS; i++) {
                if (!state.aliens.bullets[i].isFired) continue;
                Vector2 start = Vector2Scale(state.aliens.bullets[i].start, sf);
                Vector2 end = Vector2Scale(state.aliens.bullets[i].end, sf);
                DrawLineEx(start, end, state.aliens.bullets[i].width * sf, LIGHTGRAY);
            }
        }
    } else if (state.gameMode == MENU) {
        // handle input
        if (IsKeyPressed(KEY_M)) {
            // TODO: reset game state
            state.gameMode = GAME;
        }

        // logic
        tickMenu(&state, GetTime(), GetFrameTime());

        // render
        {
            ClearBackground(BLACK);
            i32 sf = state.window.scaleFactor;
            const char* todoText = "TODO: Implement";
            f32 fontSize = 10.f;
            f32 textWidth = MeasureText(todoText, fontSize);
            f32 x = (state.window.width / 2 - textWidth / 2) * sf;
            f32 y = (state.window.height / 2 - fontSize / 2) * sf;
            DrawText(todoText, x, y, fontSize * sf, WHITE);
        }
    }
}

void cleanup() {
    UnloadTexture(state.ship.texture);
    UnloadTexture(state.aliens.alienGrid[0][0].texture);
}
