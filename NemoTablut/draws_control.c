#include "draws_control.h"


void initHistoryArray(HistoryArray* history) {
    int i = 0;

    history->playedBoardSize = 0;
    history->size = 0;
    for(i = 0; i < HISTORY_DIM; i++) {
        initEmptyBoard(&(history->boards[i]));
    }
}


void addFutureBoard(HistoryArray* history, BoardState board) {
    history->boards[(history->size) % HISTORY_DIM] = board;
    history->size++;
}


void addPlayedBoard(HistoryArray* history, BoardState board) {
    history->boards[history->playedBoardSize % HISTORY_DIM] = board;
    history->playedBoardSize++;
    history->size = history->playedBoardSize;
}


int isDraw(HistoryArray* history, BoardState board) {
    int i;
    for (i = 0; i < history->size; i++) {
        if (isEqual(history->boards[i % HISTORY_DIM], board))
            return 1;
    }
    return 0;
}
