#include <raylib.h>
#include <raymath.h>
#include <stdio.h>

#include "types.h"

#define ALIEN_ROWS 5
#define ALIEN_COLS 11

typedef struct {
    Texture texture;
    Vector2 pos;
} Alien;

typedef struct {
    i32 time;
    struct {
        i32 width;
        i32 height;
        i32 scaleFactor;
        i32 targetFps;
    } window;
    struct {
        Texture texture;
        Vector2 pos;
        f32 speed;
    } ship;
    struct {
        Alien alienGrid[ALIEN_ROWS][ALIEN_COLS];
        i32 speed;
    } aliens;
} State;
State state = {0};

void printv2(Vector2 v) {
    printf("x: %f, y: %f\n", v.x, v.y);
}

void init() {
    i32 scaleFactor = 3;
    State s = {.time = 0,
               .window =
                   {
                       .width = 224 * scaleFactor,
                       .height = 256 * scaleFactor,
                       .scaleFactor = scaleFactor,
                       .targetFps = 60,
                   },
               .ship =
                   {
                       .speed = 100.f * scaleFactor,
                   },
               .aliens = {
                   .alienGrid = {0},
               }};
    state = s;

    InitWindow(s.window.width, s.window.height, "Personal Space Invaders");
    SetTargetFPS(state.window.targetFps);

    // ship
    {
        Image img = LoadImage("./res/ship.png");
        state.ship.texture = LoadTextureFromImage(img);
        UnloadImage(img);

        Vector2 pos = {5.f * scaleFactor,
                       state.window.height -
                           scaleFactor * (state.ship.texture.height + 5.f)};
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

void update() {
    ClearBackground(BLACK);
    i32 sf = state.window.scaleFactor;

    f32 m = state.ship.speed * GetFrameTime();
    if (IsKeyDown(KEY_LEFT))
        state.ship.pos.x -= m;
    if (IsKeyDown(KEY_RIGHT))
        state.ship.pos.x += m;

    // draw ship
    DrawTextureEx(state.ship.texture, state.ship.pos, 0, sf, WHITE);

    // draw aliens
    {
        f32 spacing = 5.f, offset = 20.f;
        for (int r = 0; r < ALIEN_ROWS; r++) {
            for (int c = 0; c < ALIEN_COLS; c++) {
                Alien a = state.aliens.alienGrid[r][c];
                Vector2 newpos = {(a.texture.width + spacing) * c * sf, (a.texture.height + spacing) * r * sf};
                newpos.y += offset;
                newpos.x += offset;
                a.pos = newpos;
                DrawTextureEx(a.texture, a.pos, 0, sf, WHITE);
            }
        }
    }
}

void cleanup() {
    UnloadTexture(state.ship.texture);
    UnloadTexture(state.aliens.alienGrid[0][0].texture);
}

int main(void) {
    init();
    while (!WindowShouldClose()) {
        BeginDrawing();
        update();
        EndDrawing();
    }
    CloseWindow();
    cleanup();
    return 0;
}
