#include "game.h"
#include "raylib.h"

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
