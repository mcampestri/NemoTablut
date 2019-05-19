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

    alpha = ALPHA_MIN;


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

    // TODO: condizione reinserita (controllare)
    //trovato un exact value con depthLeft >
    if(entry.type == PV_NODE && entry.deepLeft >= depthLeft) {
    	return score;
    }

    ////trovato un cut value con depthLeft>
    if(entry.type == CUT_NODE && entry.deepLeft >= depthLeft)
        alpha = score;
    //controllo se tagliare
    if (alpha >= beta)
        return beta;


    /*---------------ALGORITMO MIN MAX---------------------------------------*/


    if (isTimeOver())
        pthread_exit(NULL);

    movesNumber = getPossibleMovesWhite(board, movesArr);

    if (entry.type != -1) {    // Put first
        for (i = 0; i < movesNumber; i++) {
            if (movesArr[i] == bestMove) {
                movesArr[i] = movesArr[0];
                movesArr[0] = bestMove;
            }
        }
    }

    if (depthLeft > 1) {
        if (entry.type != -1)
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
                addWhiteEntry(&board, currentMove, alpha, depthLeft, PV_NODE);
            else
                addWhiteEntry(&board, currentMove, alpha, depthLeft, CUT_NODE);
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
                addWhiteEntry(&board, currentMove, alpha, depthLeft, PV_NODE);
            else
                addWhiteEntry(&board, currentMove, alpha, depthLeft, CUT_NODE);
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
    addWhiteEntry(&board, bestMove, alpha, depthLeft, PV_NODE);
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

    beta = BETA_MAX;

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

    // TODO: condizione reinserita (controllare)
    //trovato un exact value con depthLeft>
    if(entry.type == PV_NODE && entry.deepLeft >= depthLeft) {
        return score;
    }

    ////trovato un cut value con depthLeft>
    if(entry.type == CUT_NODE && entry.deepLeft >= depthLeft)
        beta = score;
    //controllo se tagliare
    if (alpha >= beta)
        return alpha;


    /*---------------ALGORITMO MIN MAX---------------------------------------*/


    if(isTimeOver()) {
        pthread_exit(NULL);
    }


    movesNumber = getPossibleMovesBlack(board, movesArr);

    if (entry.type != -1) {    // Put first
        for(i = 0; i < movesNumber; i++) {
            if(movesArr[i] == bestMove) {
                movesArr[i] = movesArr[0];
                movesArr[0] = bestMove;
            }
        }
    }

    if(depthLeft > 1) {
        if(entry.type != -1)
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
                addBlackEntry(&board, currentMove, beta, depthLeft, PV_NODE);
            else
                addBlackEntry(&board, currentMove, beta, depthLeft, CUT_NODE);
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
                addBlackEntry(&board, currentMove, beta, depthLeft, PV_NODE);
            else
                addBlackEntry(&board, currentMove, beta, depthLeft, CUT_NODE);
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
    addBlackEntry(&board, bestMove, beta, depthLeft, PV_NODE);
    unlockTranspositionEntryBlack(boardHash);

    return beta;
}
