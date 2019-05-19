#pragma once

#include "board.h"

char columnToServerFormat(uint32_t column);
uint32_t rowToServerFormat(uint32_t row);

void gameLoop(int playerColor, int maxTime, int numThreads);
