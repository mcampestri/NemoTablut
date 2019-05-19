#include "search_threads.h"
#include "minmax.h"

#include <pthread.h>
#include <stdio.h>


void* doSearchMax(void *arg) {
    int depth;
    ScoreType  score;
    SearchArgs* searchArgs = (SearchArgs*)arg;
    BoardState board = searchArgs->board;
    int maxDepth = searchArgs->maxDepth;
    HistoryArray historyArray = searchArgs->historyArray;

    struct timeb start, end;
    int diff;
    ftime(&start);


    for (depth = 1; depth <= maxDepth; depth++) {
        score = alphaBetaMax(board, ALPHA_MIN, BETA_MAX, depth, historyArray);
        updatePartialBestMoveWhite(&board, depth, score);

        ftime(&end);
        diff = (int)(1000.0 * (end.time - start.time) + (end.millitm - start.millitm));
        printf("Finito thread: %lu (depth: %d)\nOperation took %u milliseconds\n\n", pthread_self(), depth, diff);
    }

    ftime(&end);
    diff = (int)(1000.0 * (end.time - start.time) + (end.millitm - start.millitm));
    printf("Finito thread: %lu (depth: %d)\nOperation took %u milliseconds\n\n", pthread_self(), maxDepth, diff);

    return NULL;
}


void* doSearchMin(void *arg) {
    int depth, diff;
    ScoreType score;
    SearchArgs* searchArgs = (SearchArgs*)arg;
    BoardState board = searchArgs->board;
    int maxDepth = searchArgs->maxDepth;
    HistoryArray historyArray = searchArgs->historyArray;

    struct timeb start, end;
    ftime(&start);

    for (depth = 1; depth <= maxDepth; depth++) {
        score = alphaBetaMin(board, ALPHA_MIN, BETA_MAX, depth, historyArray);
        updatePartialBestMoveBlack(&board, depth, score);

        ftime(&end);
        diff = (int)(1000.0 * (end.time - start.time) + (end.millitm - start.millitm));
        printf("Finito thread: %lu (depth: %d)\nOperation took %u milliseconds\n\n", pthread_self(), depth, diff);
    }

    ftime(&end);
    diff = (int)(1000.0 * (end.time - start.time) + (end.millitm - start.millitm));
    printf("Finito thread: %lu (max depth: %d)\nOperation took %u milliseconds\n\n", pthread_self(), maxDepth, diff);

    return NULL;
}
