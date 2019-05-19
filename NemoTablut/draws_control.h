#pragma once

#include "board.h"

#define HISTORY_DIM 50


typedef struct {
    BoardState boards[HISTORY_DIM];
    int size;
    int playedBoardSize;
} HistoryArray;


void initHistoryArray(HistoryArray* history);
void addFutureBoard(HistoryArray* history, BoardState board);
void addPlayedBoard(HistoryArray* history, BoardState board);
int isDraw(HistoryArray* history, BoardState board);