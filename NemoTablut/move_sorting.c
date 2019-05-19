#include "move_sorting.h"
#include "search.h"

#include <stdlib.h>



typedef struct {
	Move move;
	ScoreType score;
} EvalMove;



int moveComparatorWhite(const void* a, const void* b) {
	ScoreType scoreA = ((EvalMove*)a)->score;
	ScoreType scoreB = ((EvalMove*)b)->score;

	return scoreB - scoreA;
}

int moveComparatorBlack(const void* a, const void* b) {
	ScoreType scoreA = ((EvalMove*)a)->score;
	ScoreType scoreB = ((EvalMove*)b)->score;
	
	return scoreA - scoreB;
}


void sortMovesArrayWhite(BoardState* board, Move* arr, int size) {
	BoardState tempBoard;
	int i;
	EvalMove evalMoves[MAX_MOVES];

	for(i = 0; i < size; i++) {
		tempBoard = moveWhite(*board, arr[i]);
		evalMoves[i].move = arr[i];
		evalMoves[i].score = evaluateForWhite(tempBoard);
	}

	qsort(evalMoves, size, sizeof(EvalMove), moveComparatorWhite);

	for(i = 0; i < size; i++) {
		arr[i] = evalMoves[i].move;
	}
}

void sortMovesArrayBlack(BoardState* board, Move* arr, int size) {
	BoardState tempBoard;
	int i;
	EvalMove evalMoves[MAX_MOVES];

	for(i = 0; i < size; i++) {
		tempBoard = moveBlack(*board, arr[i]);
		evalMoves[i].move = arr[i];
		evalMoves[i].score = evaluateForBlack(tempBoard);
	}

	qsort(evalMoves, size, sizeof(EvalMove), moveComparatorBlack);

	for(i = 0; i < size; i++) {
		arr[i] = evalMoves[i].move;
	}
}



/*
int historyHeuristicWhite(const void* a, const void* b) {
    Move moveA = *((Move*)a);
    Move moveB = *((Move*)b);

	int64_t scoreA = getMoveScoreWhite(moveA);
	int64_t scoreB = getMoveScoreWhite(moveB);
	
	return scoreB - scoreA;
}


int historyHeuristicBlack(const void* a, const void* b) {
    Move moveA = *((Move*)a);
    Move moveB = *((Move*)b);

    int64_t scoreA = getMoveScoreBlack(moveA);
    int64_t scoreB = getMoveScoreBlack(moveB);
	
	return scoreA - scoreB;
}


void sortMovesArrayWhite(BoardState* board, Move* arr, int size) {
	qsort(arr, size, sizeof(Move), historyHeuristicWhite);
}


void sortMovesArrayBlack(BoardState* board, Move* arr, int size) {
	qsort(arr, size, sizeof(Move), historyHeuristicBlack);
}
*/
