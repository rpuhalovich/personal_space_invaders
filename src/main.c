#include "game.h"
#include "raylib.h"

i32 main(void) {
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
