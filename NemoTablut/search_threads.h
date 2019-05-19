#pragma once

#include "board.h"
#include "draws_control.h"

typedef struct {
    BoardState board;
    int maxDepth;
    HistoryArray historyArray;
} SearchArgs;


void* doSearchMax(void *arg);
void* doSearchMin(void *arg);
