#ifndef __SNAKE_H
#define __SNAKE_H

#include <Arduino.h>
#include "button.h"

void Snake_Init();
void Snake_Reset();
void Snake_SetDirection(Direction dir);
void Snake_Update();
bool Snake_IsAlive();
int Snake_GetScore();

#endif
