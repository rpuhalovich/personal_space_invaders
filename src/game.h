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
    bool isDed;
} Alien;

typedef struct {
    Vector2 start;
    Vector2 end;
    f32 speed;
    f32 height;
    f32 width;
    bool isFired;
} Bullet;

typedef struct {
    i32 time;
    i32 score;
    struct {
        f32 margin;
        f32 leftMargin;
        f32 rightMargin;
        f32 deathFloor;
        f32 width;
        f32 height;
        f32 scaleFactor;
        f32 fontSize;
        i32 targetFps;
    } window;
    struct {
        Texture texture;
        Vector2 pos;
        Bullet bullet;
        i32 lives;
        f32 speed;
        f32 leftBoundry;
        f32 rightBoundry;
    } ship;
    struct {
        Alien alienGrid[ALIEN_ROWS][ALIEN_COLS];
        i32 alienRows;
        i32 alienCols;
        f32 spacing;
        i32 speed;
        f32 offset;
        bool isLeft;
        i32 killScore;
        bool isPaused;
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
