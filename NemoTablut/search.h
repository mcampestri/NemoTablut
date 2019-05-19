#pragma once

#include "board.h"
#include "move_type.h"
#include <time.h>
#include <stdlib.h>
#include <sys/timeb.h>

#include "search.h"
#include "moves.h"
#include "evaluation.h"
#include "move_type.h"
#include "traspostition_table.h"
#include "draws_control.h"
#include <stdlib.h>

#define ALPHA_MIN INT16_MIN
#define BETA_MAX INT16_MAX

#define MAX_MOVES 256

int expanded;


void iterativeDeepeningSearchWhite(BoardState board, int maxDepth, HistoryArray historyArray, int maxTime, int numThreads);
void iterativeDeepeningSearchBlack(BoardState board, int maxDepth, HistoryArray historyArray, int maxTime, int numThreads);
