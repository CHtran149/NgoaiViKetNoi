#include "snake.h"
#include "bientoancuc.h"

static const uint8_t MAU_COT[8] = {
    0x80, 0x40, 0x20, 0x10,
    0x08, 0x04, 0x02, 0x01
};

static const uint8_t MAU_HANG[8] = {
    0xFE, 0xFD, 0xFB, 0xF7,
    0xEF, 0xDF, 0xBF, 0x7F
};

static uint8_t snakeX[64];
static uint8_t snakeY[64];
static uint8_t snakeLength;
static int8_t foodX;
static int8_t foodY;
static Direction currentDir;
static bool alive;
static unsigned long lastMoveMillis;
static const unsigned long MOVE_INTERVAL = 1000;

static bool isSnakeAt(uint8_t x, uint8_t y)
{
    for (uint8_t i = 0; i < snakeLength; i++) {
        if (snakeX[i] == x && snakeY[i] == y) {
            return true;
        }
    }
    return false;
}

static void spawnFood()
{
    uint8_t freeCells = 0;
    uint8_t freeX[64];
    uint8_t freeY[64];

    for (uint8_t y = 0; y < 8; y++) {
        for (uint8_t x = 0; x < 8; x++) {
            if (!isSnakeAt(x, y)) {
                freeX[freeCells] = x;
                freeY[freeCells] = y;
                freeCells++;
            }
        }
    }

    if (freeCells == 0) {
        foodX = -1;
        foodY = -1;
        return;
    }

    uint8_t index = random(freeCells);
    foodX = freeX[index];
    foodY = freeY[index];
}

static void drawMatrix()
{
    for (uint8_t i = 0; i < 8; i++) {
        hang[i] = 0xFF;
        cot[i] = 0x00;
    }

    for (uint8_t i = 0; i < snakeLength; i++) {
        uint8_t x = snakeX[i];
        uint8_t y = snakeY[i];
        hang[y] = MAU_HANG[y];
        cot[y] |= MAU_COT[x];
    }

    if (foodX >= 0 && foodY >= 0) {
        hang[foodY] = MAU_HANG[foodY];
        cot[foodY] |= MAU_COT[foodX];
    }
}

void Snake_Init()
{
    randomSeed(micros());
    Snake_Reset();
}

void Snake_Reset()
{
    snakeLength = 3;
    snakeX[0] = 4; snakeY[0] = 4;
    snakeX[1] = 3; snakeY[1] = 4;
    snakeX[2] = 2; snakeY[2] = 4;
    currentDir = DIR_RIGHT;
    alive = true;
    lastMoveMillis = millis();
    spawnFood();
    drawMatrix();
}

void Snake_SetDirection(Direction dir)
{
    if (dir == DIR_NONE || !alive) {
        return;
    }

    if (dir == DIR_UP && currentDir != DIR_DOWN) {
        currentDir = DIR_UP;
    } else if (dir == DIR_DOWN && currentDir != DIR_UP) {
        currentDir = DIR_DOWN;
    } else if (dir == DIR_LEFT && currentDir != DIR_RIGHT) {
        currentDir = DIR_LEFT;
    } else if (dir == DIR_RIGHT && currentDir != DIR_LEFT) {
        currentDir = DIR_RIGHT;
    }
}

void Snake_Update()
{
    if (!alive) {
        return;
    }

    unsigned long now = millis();
    if (now - lastMoveMillis < MOVE_INTERVAL) {
        return;
    }
    lastMoveMillis = now;

    int8_t newX = snakeX[0];
    int8_t newY = snakeY[0];

    switch (currentDir) {
        case DIR_UP:    newY--; break;
        case DIR_DOWN:  newY++; break;
        case DIR_LEFT:  newX--; break;
        case DIR_RIGHT: newX++; break;
        default: break;
    }

    if (newX < 0 || newX > 7 || newY < 0 || newY > 7) {
        alive = false;
        return;
    }

    bool willGrow = (newX == foodX && newY == foodY);
    uint8_t endIndex = willGrow ? snakeLength : snakeLength - 1;
    for (uint8_t i = 0; i < endIndex; i++) {
        if (snakeX[i] == newX && snakeY[i] == newY) {
            alive = false;
            return;
        }
    }

    for (int i = snakeLength; i > 0; i--) {
        snakeX[i] = snakeX[i - 1];
        snakeY[i] = snakeY[i - 1];
    }
    snakeX[0] = newX;
    snakeY[0] = newY;

    if (willGrow) {
        if (snakeLength < 64) {
            snakeLength++;
        }
        spawnFood();
    } else {
        // Move by trimming tail: implicit because we shifted values and keep length
    }

    drawMatrix();
}

bool Snake_IsAlive()
{
    return alive;
}

int Snake_GetScore()
{
    return snakeLength - 3;
}
