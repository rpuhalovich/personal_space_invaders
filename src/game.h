#ifndef GAME_H
#define GAME_H

#include <raylib.h>
#include <raymath.h>

#include "types.h"

#define ALIEN_ROWS 5
#define ALIEN_COLS 11
#define SCALE_FACTOR 3

typedef struct {
    Texture texture;
    Vector2 pos;
} Alien;

typedef struct {
    i32 time;
    struct {
        f32 leftRightMargin;
        f32 width;
        f32 height;
        f32 scaleFactor;
        i32 targetFps;
    } window;
    struct {
        Texture texture;
        Vector2 pos;
        f32 speed;
        f32 leftBoundry;
        f32 rightBoundry;
    } ship;
    struct {
        Alien alienGrid[ALIEN_ROWS][ALIEN_COLS];
        i32 speed;
        f32 alienOffset;
        bool isLeft;
        f32 yOffset;
    } aliens;
} State;

// sets up the game state, resources and window
void init(void);

// takes in input and calculates new state
// need to separate three stages of rendering
// input > logic > render
void update(void);

void cleanup();

#endif // GAME_H
