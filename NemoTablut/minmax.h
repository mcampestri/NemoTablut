#pragma once

#include "search.h"
#include "draws_control.h"


// Numero massimo di mosse possibili ogni scacchiera
#define MAX_MOVES 256


int alphaBetaMax(BoardState board, int alpha, int beta, int depthLeft, HistoryArray historyArray);
int alphaBetaMin(BoardState board, int alpha, int beta, int depthLeft, HistoryArray historyArray);
