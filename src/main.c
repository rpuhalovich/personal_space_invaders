#include <raylib.h>
#include <raymath.h>

#include "types.h"

const i16 width = 224;
const i16 height = 256;
const i16 scaleFactor = 2;
const i16 targetFps = 60;

typedef struct {
    i16 width;
    i16 height;
    i16 scaleFactor;
    i16 targetFps;
} Window;

typedef struct {
    i32 time;
    Window window;
} State;
State state = {0};

void init() {
    State s = {
        .time = 0,
        .window.width = 224,
        .window.height = 256,
        .window.scaleFactor = 3,
    };
    state = s;
    InitWindow(s.window.width * s.window.scaleFactor, s.window.height * s.window.scaleFactor,
               "Personal Space Invaders");
    SetTargetFPS(targetFps);
}

void update() {
    ClearBackground(BLACK);
    i32 sf = state.window.scaleFactor;
    DrawRectangle(0 * sf, 0 * sf, 100 * sf, 100 * sf, WHITE);

    // draw spaceship
    {

    }
}

int main(void) {
    init();
    while (!WindowShouldClose()) {
        BeginDrawing();
        update();
        EndDrawing();
    }
    CloseWindow();
    return 0;
}
