#include <pthread.h>
#include "minmax.h"
#include "move_sorting.h"
#include "abdada.h"
#include "time_control.h"


int alphaBetaMax(BoardState board, int alpha, int beta, int depthLeft, HistoryArray historyArray) {
    int i, eval;
    Move currentMove;
    BoardState nextBoard;
    uint64_t boardHash;
    Move bestMove = -1;
    int score = ALPHA_MIN;
    int movesNumber = 0;
    Move movesArr[MAX_MOVES];

    int maxScore = ALPHA_MIN;


    if (isDraw(&historyArray, board)) {
        return 0;
    }
    addFutureBoard(&historyArray, board);

    eval = evaluateForBlack(board);

    //Check vittoria nero
    if (eval == BLACK_WIN) {        // Vittoria nero
        return BLACK_WIN;
    }
    //Check profondità raggiunta
    if (depthLeft == 0) {
        return eval;
    }

    boardHash = stringHash(&board);
    // Cerco nella trasposition Table
    // Il lock è fatto nella funzione getWhiteTransposition
    Entry entry = getWhiteTrasposition(&board, &bestMove, &score);

    int entryValid = (entry.type != -1) &&
					 ((GET_CELL_STATE(board.Board, START_ROW(bestMove), START_COLUMN(bestMove))) == WHITE || (GET_CELL_STATE(board.Board, START_ROW(bestMove), START_COLUMN(bestMove))) == WHITE_KING) &&
					 ((GET_CELL_STATE(board.Board, TARGET_ROW(bestMove), TARGET_COLUMN(bestMove))) == EMPTY);

    
    // TODO: condizione reinserita (controllare)
    //trovato un exact value con depthLeft >
    if(entryValid && entry.type == PV_NODE && entry.deepLeft >= depthLeft) {
    	return score;
    }

    ////trovato un cut value con depthLeft>
    if(entryValid && entry.type == CUT_NODE && entry.deepLeft >= depthLeft)
        maxScore = score;
    //controllo se tagliare
    if (maxScore >= beta)
        return beta;


    /*---------------ALGORITMO MIN MAX---------------------------------------*/


    if (isTimeOver())
        pthread_exit(NULL);

    movesNumber = getPossibleMovesWhite(board, movesArr);

    if (entryValid && entry.type != -1) {    // Put first
        for (i = 0; i < movesNumber; i++) {
            if (movesArr[i] == bestMove) {
                movesArr[i] = movesArr[0];
                movesArr[0] = bestMove;
            }
        }
    }

    if (depthLeft > 1) {
        if (entryValid && entry.type != -1)
            sortMovesArrayWhite(&board, &(movesArr[1]), movesNumber - 1);
        else
            sortMovesArrayWhite(&board, movesArr, movesNumber);
    }


    Move deferredMoves[MAX_MOVES];
    int numDeferred = 0;

    for (i = 0; i < movesNumber; i++) {
        currentMove = movesArr[i];

        if (i > 0 && increaseOrDefer(moveHash(currentMove, boardHash), depthLeft)) {    // 1: defer move
            deferredMoves[numDeferred] = currentMove;
            numDeferred++;
            continue;
        }

        nextBoard = moveWhite(board, currentMove);
        
        score = alphaBetaMin(nextBoard, alpha, beta, depthLeft - 1, historyArray);

        if(score > maxScore) {
        	maxScore = score;
        	bestMove = currentMove;
        }

        if (alpha < score) {
            alpha = score;
            bestMove = currentMove;
        }
        if (beta <= alpha) {
            if (entry.type != -1) {
                lockTranspositionEntryWhite(entry.hash);
                removeWhiteEntry(entry.hash);
                unlockTranspositionEntryWhite(entry.hash);
            }
            lockTranspositionEntryWhite(boardHash);
            if (alpha == WHITE_WIN)
                addWhiteEntry(&board, currentMove, maxScore, depthLeft, PV_NODE);
            else
                addWhiteEntry(&board, currentMove, maxScore, depthLeft, CUT_NODE);
            unlockTranspositionEntryWhite(boardHash);


            endSearch(moveHash(currentMove, boardHash), depthLeft);

            return beta;
        }

        endSearch(moveHash(currentMove, boardHash), depthLeft);
    }

    for (i = 0; i < numDeferred; i++) {
        currentMove = deferredMoves[i];
        nextBoard = moveWhite(board, currentMove);
        
        score = alphaBetaMin(nextBoard, alpha, beta, depthLeft - 1, historyArray);

        if(score > maxScore) {
        	maxScore = score;
        	bestMove = currentMove;
        }

        if (alpha < score) {
            alpha = score;
            bestMove = currentMove;
        }

        if (beta <= alpha) {
            if (entry.type != -1) {
                lockTranspositionEntryWhite(entry.hash);
                removeWhiteEntry(entry.hash);
                unlockTranspositionEntryWhite(entry.hash);
            }
            lockTranspositionEntryWhite(boardHash);
            if (alpha == WHITE_WIN)
                addWhiteEntry(&board, currentMove, maxScore, depthLeft, PV_NODE);
            else
                addWhiteEntry(&board, currentMove, maxScore, depthLeft, CUT_NODE);
            unlockTranspositionEntryWhite(boardHash);


            return beta;
        }
    }

    //Aggiornamento Table
    if (entry.type != -1) {
        lockTranspositionEntryWhite(entry.hash);
        removeWhiteEntry(entry.hash);
        unlockTranspositionEntryWhite(entry.hash);
    }
    lockTranspositionEntryWhite(boardHash);
    addWhiteEntry(&board, bestMove, maxScore, depthLeft, PV_NODE);
    unlockTranspositionEntryWhite(boardHash);


    return alpha;
}



int alphaBetaMin(BoardState board, int alpha, int beta, int depthLeft, HistoryArray historyArray) {
    int i, eval;
    Move currentMove;
    Move movesArr[MAX_MOVES];
    BoardState nextBoard;
    uint64_t boardHash;
    Move bestMove = -1;
    int score = ALPHA_MIN;
    int movesNumber = 0;

    int minScore = BETA_MAX;

    if(isDraw(&historyArray, board)) {
        return 0;
    }
    addFutureBoard(&historyArray, board);

    eval = evaluateForWhite(board);

    //Check vittoria bianco
    if (eval == WHITE_WIN) {		// Vittoria nero
        return WHITE_WIN;
    }
    //Check profondità raggiunta
    if (depthLeft == 0) {
        return eval;
    }

    boardHash = stringHash(&board);
    //Cerco nella trasposition Table
    Entry entry = getBlackTrasposition(&board, &bestMove, &score);

    int entryValid = (entry.type != -1) &&
					 ((GET_CELL_STATE(board.Board, START_ROW(bestMove), START_COLUMN(bestMove))) == BLACK) &&
					 ((GET_CELL_STATE(board.Board, TARGET_ROW(bestMove), TARGET_COLUMN(bestMove))) == EMPTY);
    
    // TODO: condizione reinserita (controllare)
    //trovato un exact value con depthLeft>
    if(entryValid && entry.type == PV_NODE && entry.deepLeft >= depthLeft) {
        return score;
    }

    ////trovato un cut value con depthLeft>
    if(entryValid && entry.type == CUT_NODE && entry.deepLeft >= depthLeft)
        minScore = score;
    //controllo se tagliare
    if (alpha >= minScore)
        return alpha;


    /*---------------ALGORITMO MIN MAX---------------------------------------*/


    if(isTimeOver()) {
        pthread_exit(NULL);
    }


    movesNumber = getPossibleMovesBlack(board, movesArr);

    if (entryValid && entry.type != -1) {    // Put first
        for(i = 0; i < movesNumber; i++) {
            if(movesArr[i] == bestMove) {
                movesArr[i] = movesArr[0];
                movesArr[0] = bestMove;
            }
        }
    }

    if(depthLeft > 1) {
        if(entryValid && entry.type != -1)
            sortMovesArrayBlack(&board, &(movesArr[1]), movesNumber - 1);
        else
            sortMovesArrayBlack(&board, movesArr, movesNumber);
    }


    Move deferredMoves[MAX_MOVES];
    int numDeferred = 0;

    for(i = 0; i < movesNumber; i++) {
        currentMove = movesArr[i];

        if (i > 0 && increaseOrDefer(moveHash(currentMove, boardHash), depthLeft)) {    // 1: defer move
            deferredMoves[numDeferred] = currentMove;
            numDeferred++;
            continue;
        }

        nextBoard = moveBlack(board, currentMove);

        score = alphaBetaMax(nextBoard, alpha, beta, depthLeft - 1, historyArray);

        if(score < minScore) {
        	minScore = score;
        	bestMove = currentMove;
        }

        if (beta > score) {
            beta = score;
            bestMove = currentMove;
        }
        if (beta <= alpha) {
            if (entry.type != -1) {
                lockTranspositionEntryBlack(entry.hash);
                removeBlackEntry(entry.hash);
                unlockTranspositionEntryBlack(entry.hash);
            }
            lockTranspositionEntryBlack(boardHash);
            if (beta == BLACK_WIN)
                addBlackEntry(&board, currentMove, minScore, depthLeft, PV_NODE);
            else
                addBlackEntry(&board, currentMove, minScore, depthLeft, CUT_NODE);
            unlockTranspositionEntryBlack(boardHash);


            endSearch(moveHash(currentMove, boardHash), depthLeft);

            return alpha;
        }

        endSearch(moveHash(currentMove, boardHash), depthLeft);
    }

    for (i = 0; i < numDeferred; i++) {
        currentMove = deferredMoves[i];
        nextBoard = moveBlack(board, currentMove);
        
        score = alphaBetaMax(nextBoard, alpha, beta, depthLeft - 1, historyArray);

        if(score < minScore) {
        	minScore = score;
        	bestMove = currentMove;
        }

        if (beta > score) {
            beta = score;
            bestMove = currentMove;
        }
        if (beta <= alpha) {
            if (entry.type != -1) {
                lockTranspositionEntryBlack(entry.hash);
                removeBlackEntry(entry.hash);
                unlockTranspositionEntryBlack(entry.hash);
            }
            lockTranspositionEntryBlack(boardHash);
            if (beta == BLACK_WIN)
                addBlackEntry(&board, currentMove, minScore, depthLeft, PV_NODE);
            else
                addBlackEntry(&board, currentMove, minScore, depthLeft, CUT_NODE);
            unlockTranspositionEntryBlack(boardHash);


            return alpha;
        }
    }

    //Aggiornamento Table
    if (entry.type != -1) {
        lockTranspositionEntryBlack(entry.hash);
        removeBlackEntry(entry.hash);
        unlockTranspositionEntryBlack(entry.hash);
    }
    lockTranspositionEntryBlack(boardHash);
    addBlackEntry(&board, bestMove, minScore, depthLeft, PV_NODE);
    unlockTranspositionEntryBlack(boardHash);

    return beta;
}
