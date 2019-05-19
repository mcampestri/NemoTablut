#pragma once

#include "parameters.h"
#include "board.h"
#include "moves.h"
#include "entry.h"
#include <stdint.h>

#define MOVE_NOT_FOUND UINT16_MAX
#define SCORE_NOT_FOUND_WHITE INT16_MIN
#define SCORE_NOT_FOUND_BLACK INT16_MAX

void initTables();

void updatePartialBestMoveWhite(BoardState* board, int maxDepth, ScoreType score);
void updatePartialBestMoveBlack(BoardState* board, int maxDepth, ScoreType score);
Move getFinalBestMoveWhite();
Move getFinalBestMoveBlack();

Move getBestMoveWhite(BoardState* currentBoard);
Move getBestMoveBlack(BoardState* currentBoard);

void addWhiteEntry(BoardState* board, Move nextMove, ScoreType euristicValue, uint8_t depthLeft, uint8_t type);
void addBlackEntry(BoardState* board, Move nextMove, ScoreType euristicValue, uint8_t depthLeft, uint8_t type);
void removeWhiteEntry(uint64_t hash);
void removeBlackEntry(uint64_t hash);

Entry getWhiteTrasposition(BoardState* currentBoard, Move* bestMove, int* euristicValue);
Entry getBlackTrasposition(BoardState* currentBoard, Move* bestMove, int* euristicValue);

uint64_t stringHash(BoardState* board);

void lockTranspositionEntryWhite(uint64_t hash);
void unlockTranspositionEntryWhite(uint64_t hash);
void lockTranspositionEntryBlack(uint64_t hash);
void unlockTranspositionEntryBlack(uint64_t hash);
