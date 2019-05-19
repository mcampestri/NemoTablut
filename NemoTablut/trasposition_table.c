#include <pthread.h>
#include "traspostition_table.h"
#include "trasformation.h"
#include "search.h"
#include "moves.h"

typedef struct {
    int depth;
    Move move;
    pthread_spinlock_t lock;
    ScoreType score;
} PartialBestMove;


//////////////////////////////////////////////////////////////////////
// Global variables
TraspositionRow WHITE_TABLE[TRANSPOSITION_TABLE_DIM];
TraspositionRow BLACK_TABLE[TRANSPOSITION_TABLE_DIM];

pthread_spinlock_t TABLE_LOCK_WHITE[TRANSPOSITION_TABLE_DIM];
pthread_spinlock_t TABLE_LOCK_BLACK[TRANSPOSITION_TABLE_DIM];

PartialBestMove partialBestMoveWhite;
PartialBestMove partialBestMoveBlack;
int found = 0;
//////////////////////////////////////////////////////////////////////



void updatePartialBestMoveWhite(BoardState* board, int maxDepth, ScoreType score) {
    pthread_spin_lock(&(partialBestMoveWhite.lock));

    if(maxDepth >= partialBestMoveWhite.depth) {
        Move bestMove = getBestMoveWhite(board);

        if(bestMove != MOVE_NOT_FOUND && bestMove != 0) {
            printMove(partialBestMoveWhite.move);
            printf("   ==>   ");
            printMove(bestMove);
            printf("\n");
            printf("DEPTH:\t%d  ==>  %d\n", partialBestMoveWhite.depth, maxDepth);
            printf("SCORE:\t%d  ==>  %d\n", partialBestMoveWhite.score, score);

            partialBestMoveWhite.move = bestMove;
            partialBestMoveWhite.depth = maxDepth;
            partialBestMoveWhite.score = score;
        }
    }

    pthread_spin_unlock(&(partialBestMoveWhite.lock));
}

void updatePartialBestMoveBlack(BoardState* board, int maxDepth, ScoreType score) {
    pthread_spin_lock(&(partialBestMoveBlack.lock));

    if(maxDepth >= partialBestMoveBlack.depth) {
        Move bestMove = getBestMoveBlack(board);

        if(bestMove != MOVE_NOT_FOUND && bestMove != 0) {
            printMove(partialBestMoveBlack.move);
            printf("   ==>   ");
            printMove(bestMove);
            printf("\n");
            printf("DEPTH:\t%d  ==>  %d\n", partialBestMoveBlack.depth, maxDepth);
            printf("SCORE:\t%d  ==>  %d\n", partialBestMoveBlack.score, score);

            partialBestMoveBlack.move = bestMove;
            partialBestMoveBlack.depth = maxDepth;
            partialBestMoveBlack.score = score;
        }
    }

    pthread_spin_unlock(&(partialBestMoveBlack.lock));
}


Move getFinalBestMoveWhite() {
    Move result = partialBestMoveWhite.move;
    partialBestMoveWhite.move = MOVE_NOT_FOUND;
    partialBestMoveWhite.depth = 0;
    partialBestMoveWhite.score = INT16_MIN;

    return result;
}

Move getFinalBestMoveBlack() {
    Move result = partialBestMoveBlack.move;
    partialBestMoveBlack.move = MOVE_NOT_FOUND;
    partialBestMoveBlack.depth = 0;
    partialBestMoveBlack.score = INT16_MAX;

    return result;
}


void initTables() {
    int i;
    for (i = 0; i < TRANSPOSITION_TABLE_DIM; i++) {
        BLACK_TABLE[i].dim = 0;
        WHITE_TABLE[i].dim = 0;

        pthread_spin_init(&(TABLE_LOCK_WHITE[i]), PTHREAD_PROCESS_PRIVATE);
        pthread_spin_init(&(TABLE_LOCK_BLACK[i]), PTHREAD_PROCESS_PRIVATE);
    }

    partialBestMoveWhite.move = MOVE_NOT_FOUND;
    partialBestMoveWhite.depth = 0;
    partialBestMoveWhite.score = INT16_MIN;
    pthread_spin_init(&(partialBestMoveWhite.lock), PTHREAD_PROCESS_PRIVATE);

    partialBestMoveBlack.move = MOVE_NOT_FOUND;
    partialBestMoveBlack.depth = 0;
    partialBestMoveBlack.score = INT16_MAX;
    pthread_spin_init(&(partialBestMoveBlack.lock), PTHREAD_PROCESS_PRIVATE);
}


void replaceEntry(TraspositionRow* riga, Entry nuovaEntry) {
    int i;
    for (i = 0; i < riga->dim-1; i++) {
        riga->row[i] = riga->row[i + 1];
    }
    riga->row[i] = nuovaEntry;
}


void addBlackEntry(BoardState* board, Move nextMove, ScoreType euristicValue, uint8_t depthLeft, uint8_t type) {
    uint64_t hash = stringHash(board);
    int black_index = hash % TRANSPOSITION_TABLE_DIM;
    Entry newElement = { hash, nextMove, euristicValue, depthLeft, type };
    if (BLACK_TABLE[black_index].dim == TRANSPOSITION_TABLE_WIDTH)
        replaceEntry(&BLACK_TABLE[black_index], newElement);
    else
        BLACK_TABLE[black_index].row[BLACK_TABLE[black_index].dim++] = newElement;
}


void addWhiteEntry(BoardState* board, Move nextMove, ScoreType euristicValue, uint8_t depthLeft, uint8_t type) {
    uint64_t hash = stringHash(board);
    int white_index = hash % TRANSPOSITION_TABLE_DIM;
    Entry newElement = { hash, nextMove, euristicValue, depthLeft, type };
    if (WHITE_TABLE[white_index].dim == TRANSPOSITION_TABLE_WIDTH)
        replaceEntry(&WHITE_TABLE[white_index], newElement);
    else
        WHITE_TABLE[white_index].row[WHITE_TABLE[white_index].dim++] = newElement;
}


void removeWhiteEntry(uint64_t hash) {
    TraspositionRow* riga = &WHITE_TABLE[hash % TRANSPOSITION_TABLE_DIM];
    int i = 0;
    for (i = 0; i < riga->dim - 1; i++) {
        if (riga->row[i].hash == hash) {
            for (; i < riga->dim - 1; i++) {
                riga->row[i] = riga->row[i + 1];
            }
        }
    }
    riga->dim--;
}



void removeBlackEntry(uint64_t hash) {
    TraspositionRow* riga = &BLACK_TABLE[hash % TRANSPOSITION_TABLE_DIM];
    int i = 0;
    for (i = 0; i < riga->dim - 1; i++) {
        if (riga->row[i].hash == hash) {
            for (; i < riga->dim - 1; i++) {
                riga->row[i] = riga->row[i + 1];
            }
        }
    }
    riga->dim--;
}


#define A 54059 /* a prime */
#define B 76963 /* another prime */
//#define C 86969 /* yet another prime */
#define FIRSTH 37 /* also prime */

inline uint64_t stringHash(BoardState* board) {
    uint64_t hash = FIRSTH;
    int i;

    for (i = 0; i < 9; i++) {
        hash = (hash * A) ^ (((board->Board[i]) & 0b1111111111111111U) * B);
        hash = (hash * A) ^ ((board->Board[i] >> 16U) * B);
    }

    return hash;
}



//0 entry non trovata
//1 trovata entry con depth left maggiore
//trovata entry con depth left minore
Entry getWhiteTrasposition(BoardState* currentBoard, Move* bestMove, int* euristicValue) {
    TraspositionRow tempNode;
    int i;
    BoardState resultBoard, rotated90Board, rotated180Board, rotated270Board;

    uint64_t hash = stringHash(currentBoard);
    uint64_t index = hash % TRANSPOSITION_TABLE_DIM;

    CreateSymmetricalHorizontal(currentBoard, &resultBoard);
    uint64_t horizontalHash = stringHash(&resultBoard);
    CreateSymmetricalVertical(currentBoard, &resultBoard);
    uint64_t verticalHash = stringHash(&resultBoard);
    CreateSymmetricalUpToDown(currentBoard, &resultBoard);
    uint64_t upToDownHash = stringHash(&resultBoard);
    CreateSymmetricalDownToUp(currentBoard, &resultBoard);
    uint64_t downToUpHash = stringHash(&resultBoard);

    Create90degrees(currentBoard, &rotated90Board);
    uint64_t degrees90Hash = stringHash(&rotated90Board);
    Create90degrees(&rotated90Board, &rotated180Board);
    uint64_t degrees180Hash = stringHash(&rotated180Board);
    Create90degrees(&rotated180Board, &rotated270Board);
    uint64_t degrees270Hash = stringHash(&rotated270Board);

    found++;


    //caso normale
    lockTranspositionEntryWhite(hash);
    tempNode = WHITE_TABLE[index];
    for (i = 0; i < tempNode.dim; i++) {
        if ((tempNode.row[i].hash) == hash) {
            *euristicValue = tempNode.row[i].euristicValue;
            *bestMove = tempNode.row[i].nextMove;
            unlockTranspositionEntryWhite(hash);
            return tempNode.row[i];
        }
    }
    unlockTranspositionEntryWhite(hash);

    //simmetria orrizzontale
    lockTranspositionEntryWhite(horizontalHash);
    tempNode = WHITE_TABLE[horizontalHash % TRANSPOSITION_TABLE_DIM];
    for (i = 0; i < tempNode.dim; i++) {
        if ((tempNode.row[i].hash) == horizontalHash) {
            *euristicValue = tempNode.row[i].euristicValue;
            *bestMove = createSymmetricalHorizontalMove(tempNode.row[i].nextMove);
            unlockTranspositionEntryWhite(horizontalHash);
            return tempNode.row[i];
        }
    }
    unlockTranspositionEntryWhite(horizontalHash);

    //simmetria verticale
    lockTranspositionEntryWhite(verticalHash);
    tempNode = WHITE_TABLE[verticalHash % TRANSPOSITION_TABLE_DIM];
    for (i = 0; i < tempNode.dim; i++) {
        if ((tempNode.row[i].hash) == verticalHash) {
            *euristicValue = tempNode.row[i].euristicValue;
            *bestMove = createSymmetricalVerticalMove(tempNode.row[i].nextMove);
            unlockTranspositionEntryWhite(verticalHash);
            return tempNode.row[i];
        }
    }
    unlockTranspositionEntryWhite(verticalHash);

    //simmetria up to down
    lockTranspositionEntryWhite(upToDownHash);
    tempNode = WHITE_TABLE[upToDownHash % TRANSPOSITION_TABLE_DIM];
    for (i = 0; i < tempNode.dim; i++) {
        if ((tempNode.row[i].hash) == upToDownHash) {
            *euristicValue = tempNode.row[i].euristicValue;
            *bestMove = createSymmetricalUpToDownMove(tempNode.row[i].nextMove);
            unlockTranspositionEntryWhite(upToDownHash);
            return tempNode.row[i];
        }
    }
    unlockTranspositionEntryWhite(upToDownHash);

    //simmetria down to up
    lockTranspositionEntryWhite(downToUpHash);
    tempNode = WHITE_TABLE[downToUpHash % TRANSPOSITION_TABLE_DIM];
    for (i = 0; i < tempNode.dim; i++) {
        if ((tempNode.row[i].hash) == downToUpHash) {
            *euristicValue = tempNode.row[i].euristicValue;
            *bestMove = createSymmetricalDownToUpMove(tempNode.row[i].nextMove);
            unlockTranspositionEntryWhite(downToUpHash);
            return tempNode.row[i];
        }
    }
    unlockTranspositionEntryWhite(downToUpHash);

    //simmetria 90 deegres
    lockTranspositionEntryWhite(degrees90Hash);
    tempNode = WHITE_TABLE[degrees90Hash % TRANSPOSITION_TABLE_DIM];
    for (i = 0; i < tempNode.dim; i++) {
        if ((tempNode.row[i].hash) == degrees90Hash) {
            *euristicValue = tempNode.row[i].euristicValue;
            *bestMove = create90degreesMove(create90degreesMove(create90degreesMove(tempNode.row[i].nextMove)));
            unlockTranspositionEntryWhite(degrees90Hash);
            return tempNode.row[i];
        }
    }
    unlockTranspositionEntryWhite(degrees90Hash);

    //180 gradi
    lockTranspositionEntryWhite(degrees180Hash);
    tempNode = WHITE_TABLE[degrees180Hash % TRANSPOSITION_TABLE_DIM];
    for (i = 0; i < tempNode.dim; i++) {
        if ((tempNode.row[i].hash) == degrees180Hash) {
            *euristicValue = tempNode.row[i].euristicValue;
            *bestMove = create90degreesMove(create90degreesMove(tempNode.row[i].nextMove));
            unlockTranspositionEntryWhite(degrees180Hash);
            return tempNode.row[i];
        }
    }
    unlockTranspositionEntryWhite(degrees180Hash);

    //270 gradi
    lockTranspositionEntryWhite(degrees270Hash);
    tempNode = WHITE_TABLE[degrees270Hash % TRANSPOSITION_TABLE_DIM];
    for (i = 0; i < tempNode.dim; i++) {
        if ((tempNode.row[i].hash) == degrees270Hash) {
            *euristicValue = tempNode.row[i].euristicValue;
            *bestMove = create90degreesMove(tempNode.row[i].nextMove);
            unlockTranspositionEntryWhite(degrees270Hash);
            return tempNode.row[i];
        }
    }
    unlockTranspositionEntryWhite(degrees270Hash);

    found--;
    Entry defaultEntry = {0, MOVE_NOT_FOUND, SCORE_NOT_FOUND_WHITE, 0, -1};
    return defaultEntry;
}



Entry getBlackTrasposition(BoardState* currentBoard, Move* bestMove, int* euristicValue) {
    TraspositionRow tempNode;
    int i;
    BoardState resultBoard, rotated90Board, rotated180Board, rotated270Board;
    uint64_t hash = stringHash(currentBoard);
    uint64_t index = hash % TRANSPOSITION_TABLE_DIM;
    CreateSymmetricalHorizontal(currentBoard, &resultBoard);
    uint64_t horizontalHash = stringHash(&resultBoard);
    CreateSymmetricalVertical(currentBoard, &resultBoard);
    uint64_t verticalHash = stringHash(&resultBoard);
    CreateSymmetricalUpToDown(currentBoard, &resultBoard);
    uint64_t upToDownHash = stringHash(&resultBoard);
    CreateSymmetricalDownToUp(currentBoard, &resultBoard);
    uint64_t downToUpHash = stringHash(&resultBoard);

    Create90degrees(currentBoard, &rotated90Board);
    uint64_t degrees90Hash = stringHash(&rotated90Board);
    Create90degrees(&rotated90Board, &rotated180Board);
    uint64_t degrees180Hash = stringHash(&rotated180Board);
    Create90degrees(&rotated180Board, &rotated270Board);
    uint64_t degrees270Hash = stringHash(&rotated270Board);

    found++;

    //caso normale
    lockTranspositionEntryBlack(hash);
    tempNode = BLACK_TABLE[index];
    for (i = 0; i < tempNode.dim; i++) {
        if ((tempNode.row[i].hash) == hash) {
            *euristicValue = tempNode.row[i].euristicValue;
            *bestMove = tempNode.row[i].nextMove;
            unlockTranspositionEntryBlack(hash);
            return tempNode.row[i];
        }
    }
    unlockTranspositionEntryBlack(hash);

    //simmetria orrizzontale
    lockTranspositionEntryBlack(horizontalHash);
    tempNode = BLACK_TABLE[horizontalHash % TRANSPOSITION_TABLE_DIM];
    for (i = 0; i < tempNode.dim; i++) {
        if ((tempNode.row[i].hash) == horizontalHash) {
            *euristicValue = tempNode.row[i].euristicValue;
            *bestMove = createSymmetricalHorizontalMove(tempNode.row[i].nextMove);
            unlockTranspositionEntryBlack(horizontalHash);
            return tempNode.row[i];
        }
    }
    unlockTranspositionEntryBlack(horizontalHash);

    //simmetria verticale
    lockTranspositionEntryBlack(verticalHash);
    tempNode = BLACK_TABLE[verticalHash % TRANSPOSITION_TABLE_DIM];
    for (i = 0; i < tempNode.dim; i++) {
        if ((tempNode.row[i].hash) == verticalHash) {
            *euristicValue = tempNode.row[i].euristicValue;
            *bestMove = createSymmetricalVerticalMove(tempNode.row[i].nextMove);
            unlockTranspositionEntryBlack(verticalHash);
            return tempNode.row[i];
        }
    }
    unlockTranspositionEntryBlack(verticalHash);

    //simmetria up to down
    lockTranspositionEntryBlack(upToDownHash);
    tempNode = BLACK_TABLE[upToDownHash % TRANSPOSITION_TABLE_DIM];
    for (i = 0; i < tempNode.dim; i++) {
        if ((tempNode.row[i].hash) == upToDownHash) {
            *euristicValue = tempNode.row[i].euristicValue;
            *bestMove = createSymmetricalUpToDownMove(tempNode.row[i].nextMove);
            unlockTranspositionEntryBlack(upToDownHash);
            return tempNode.row[i];
        }
    }
    unlockTranspositionEntryBlack(upToDownHash);

    //simmetria down to up
    lockTranspositionEntryBlack(downToUpHash);
    tempNode = BLACK_TABLE[downToUpHash % TRANSPOSITION_TABLE_DIM];
    for (i = 0; i < tempNode.dim; i++) {
        if ((tempNode.row[i].hash) == downToUpHash) {
            *euristicValue = tempNode.row[i].euristicValue;
            *bestMove = createSymmetricalDownToUpMove(tempNode.row[i].nextMove);
            unlockTranspositionEntryBlack(downToUpHash);
            return tempNode.row[i];
        }
    }
    unlockTranspositionEntryBlack(downToUpHash);

    //simmetria 90 deegres
    lockTranspositionEntryBlack(degrees90Hash);
    tempNode = BLACK_TABLE[degrees90Hash % TRANSPOSITION_TABLE_DIM];
    for (i = 0; i < tempNode.dim; i++) {
        if ((tempNode.row[i].hash) == degrees90Hash) {
            *euristicValue = tempNode.row[i].euristicValue;
            *bestMove = create90degreesMove(create90degreesMove(create90degreesMove(tempNode.row[i].nextMove)));
            unlockTranspositionEntryBlack(degrees90Hash);
            return tempNode.row[i];
        }
    }
    unlockTranspositionEntryBlack(degrees90Hash);

    //180 gradi
    lockTranspositionEntryBlack(degrees180Hash);
    tempNode = BLACK_TABLE[degrees180Hash % TRANSPOSITION_TABLE_DIM];
    for (i = 0; i < tempNode.dim; i++) {
        if ((tempNode.row[i].hash) == degrees180Hash) {
            *euristicValue = tempNode.row[i].euristicValue;
            *bestMove = create90degreesMove(create90degreesMove(tempNode.row[i].nextMove));
            unlockTranspositionEntryBlack(degrees180Hash);
            return tempNode.row[i];
        }
    }
    unlockTranspositionEntryBlack(degrees180Hash);

    //270 gradi
    lockTranspositionEntryBlack(degrees270Hash);
    tempNode = BLACK_TABLE[degrees270Hash % TRANSPOSITION_TABLE_DIM];
    for (i = 0; i < tempNode.dim; i++) {
        if ((tempNode.row[i].hash) == degrees270Hash) {
            *euristicValue = tempNode.row[i].euristicValue;
            *bestMove = create90degreesMove(tempNode.row[i].nextMove);
            unlockTranspositionEntryBlack(degrees270Hash);
            return tempNode.row[i];
        }
    }
    unlockTranspositionEntryBlack(degrees270Hash);

    found--;
    Entry defaultEntry = {0, MOVE_NOT_FOUND, SCORE_NOT_FOUND_BLACK, 0, -1};
    return defaultEntry;
}




Move getBestMoveBlack(BoardState* currentBoard) {
    TraspositionRow tempNode;
    int i = 0;
    BoardState resultBoard, rotated90Board, rotated180Board, rotated270Board;

    uint64_t hash = stringHash(currentBoard);
    uint64_t index = hash % TRANSPOSITION_TABLE_DIM;

    CreateSymmetricalHorizontal(currentBoard, &resultBoard);
    uint64_t horizontalHash = stringHash(&resultBoard);
    CreateSymmetricalVertical(currentBoard, &resultBoard);
    uint64_t verticalHash = stringHash(&resultBoard);
    CreateSymmetricalUpToDown(currentBoard, &resultBoard);
    uint64_t upToDownHash = stringHash(&resultBoard);
    CreateSymmetricalDownToUp(currentBoard, &resultBoard);
    uint64_t downToUpHash = stringHash(&resultBoard);

    Create90degrees(currentBoard, &rotated90Board);
    uint64_t degrees90Hash = stringHash(&rotated90Board);
    Create90degrees(&rotated90Board, &rotated180Board);
    uint64_t degrees180Hash = stringHash(&rotated180Board);
    Create90degrees(&rotated180Board, &rotated270Board);
    uint64_t degrees270Hash = stringHash(&rotated270Board);

    //caso normale
    lockTranspositionEntryBlack(hash);
    tempNode = BLACK_TABLE[index];
    for (i = 0; i < tempNode.dim; i++) {
        if ((tempNode.row[i].hash) == hash) {
            unlockTranspositionEntryBlack(hash);
            return tempNode.row[i].nextMove;
        }
    }
    unlockTranspositionEntryBlack(hash);


    //simmetria orrizzontale
    lockTranspositionEntryBlack(horizontalHash);
    tempNode = BLACK_TABLE[horizontalHash % TRANSPOSITION_TABLE_DIM];
    for (i = 0; i < tempNode.dim; i++) {
        if ((tempNode.row[i].hash) == hash) {
            unlockTranspositionEntryBlack(horizontalHash);
            return createSymmetricalHorizontalMove(tempNode.row[i].nextMove);
        }
    }
    unlockTranspositionEntryBlack(horizontalHash);

    //simmetria verticale
    lockTranspositionEntryBlack(verticalHash);
    tempNode = BLACK_TABLE[verticalHash % TRANSPOSITION_TABLE_DIM];
    for (i = 0; i < tempNode.dim; i++) {
        if ((tempNode.row[i].hash) == hash) {
            unlockTranspositionEntryBlack(verticalHash);
            return createSymmetricalVerticalMove(tempNode.row[i].nextMove);
        }
    }
    unlockTranspositionEntryBlack(verticalHash);

    //simmetria up to down
    lockTranspositionEntryBlack(upToDownHash);
    tempNode = BLACK_TABLE[upToDownHash % TRANSPOSITION_TABLE_DIM];
    for (i = 0; i < tempNode.dim; i++) {
        if ((tempNode.row[i].hash) == hash) {
            unlockTranspositionEntryBlack(upToDownHash);
            return createSymmetricalUpToDownMove(tempNode.row[i].nextMove);
        }
    }
    unlockTranspositionEntryBlack(upToDownHash);

    //simmetria down to up
    lockTranspositionEntryBlack(downToUpHash);
    tempNode = BLACK_TABLE[downToUpHash % TRANSPOSITION_TABLE_DIM];
    for (i = 0; i < tempNode.dim; i++) {
        if ((tempNode.row[i].hash) == hash) {
            unlockTranspositionEntryBlack(downToUpHash);
            return createSymmetricalDownToUpMove(tempNode.row[i].nextMove);
        }
    }
    unlockTranspositionEntryBlack(downToUpHash);

    //simmetria 90 deegres
    lockTranspositionEntryBlack(degrees90Hash);
    tempNode = BLACK_TABLE[degrees90Hash % TRANSPOSITION_TABLE_DIM];
    for (i = 0; i < tempNode.dim; i++) {
        if ((tempNode.row[i].hash) == hash) {
            unlockTranspositionEntryBlack(degrees90Hash);
            return create90degreesMove(create90degreesMove(create90degreesMove(tempNode.row[i].nextMove)));
        }
    }
    unlockTranspositionEntryBlack(degrees90Hash);

    //180 gradi
    lockTranspositionEntryBlack(degrees180Hash);
    tempNode = BLACK_TABLE[degrees180Hash % TRANSPOSITION_TABLE_DIM];
    for (i = 0; i < tempNode.dim; i++) {
        if ((tempNode.row[i].hash) == hash) {
            unlockTranspositionEntryBlack(degrees180Hash);
            return create90degreesMove(create90degreesMove(tempNode.row[i].nextMove));
        }
    }
    unlockTranspositionEntryBlack(degrees180Hash);

    //270 gradi
    lockTranspositionEntryBlack(degrees270Hash);
    tempNode = BLACK_TABLE[degrees270Hash % TRANSPOSITION_TABLE_DIM];
    for (i = 0; i < tempNode.dim; i++) {
        if ((tempNode.row[i].hash) == hash) {
            unlockTranspositionEntryBlack(degrees270Hash);
            return create90degreesMove(tempNode.row[i].nextMove);
        }
    }
    unlockTranspositionEntryBlack(degrees270Hash);

    printf("MOSSA NON TROVATA\n");

    return MOVE_NOT_FOUND;
}



Move getBestMoveWhite(BoardState* currentBoard) {
    TraspositionRow tempNode;
    int i = 0;
    BoardState resultBoard, rotated90Board, rotated180Board, rotated270Board;

    uint64_t hash = stringHash(currentBoard);
    uint64_t index = hash % TRANSPOSITION_TABLE_DIM;

    CreateSymmetricalHorizontal(currentBoard, &resultBoard);
    uint64_t horizontalHash = stringHash(&resultBoard);
    CreateSymmetricalVertical(currentBoard, &resultBoard);
    uint64_t verticalHash = stringHash(&resultBoard);
    CreateSymmetricalUpToDown(currentBoard, &resultBoard);
    uint64_t upToDownHash = stringHash(&resultBoard);
    CreateSymmetricalDownToUp(currentBoard, &resultBoard);
    uint64_t downToUpHash = stringHash(&resultBoard);

    Create90degrees(currentBoard, &rotated90Board);
    uint64_t degrees90Hash = stringHash(&rotated90Board);
    Create90degrees(&rotated90Board, &rotated180Board);
    uint64_t degrees180Hash = stringHash(&rotated180Board);
    Create90degrees(&rotated180Board, &rotated270Board);
    uint64_t degrees270Hash = stringHash(&rotated270Board);

    //caso normale
    lockTranspositionEntryWhite(hash);
    tempNode = WHITE_TABLE[index];
    for (i = 0; i < tempNode.dim; i++) {
        if ((tempNode.row[i].hash) == hash) {
            unlockTranspositionEntryWhite(hash);
            return tempNode.row[i].nextMove;
        }
    }
    unlockTranspositionEntryWhite(hash);

    //simmetria orrizzontale
    lockTranspositionEntryWhite(horizontalHash);
    tempNode = WHITE_TABLE[horizontalHash % TRANSPOSITION_TABLE_DIM];
    for (i = 0; i < tempNode.dim; i++) {
        if ((tempNode.row[i].hash) == hash) {
            unlockTranspositionEntryWhite(horizontalHash);
            return createSymmetricalHorizontalMove(tempNode.row[i].nextMove);
        }
    }
    unlockTranspositionEntryWhite(horizontalHash);

    //simmetria verticale
    lockTranspositionEntryWhite(verticalHash);
    tempNode = WHITE_TABLE[verticalHash % TRANSPOSITION_TABLE_DIM];
    for (i = 0; i < tempNode.dim; i++) {
        if ((tempNode.row[i].hash) == hash) {
            unlockTranspositionEntryWhite(verticalHash);
            return createSymmetricalVerticalMove(tempNode.row[i].nextMove);
        }
    }
    unlockTranspositionEntryWhite(verticalHash);

    //simmetria up to down
    lockTranspositionEntryWhite(upToDownHash);
    tempNode = WHITE_TABLE[upToDownHash % TRANSPOSITION_TABLE_DIM];
    for (i = 0; i < tempNode.dim; i++) {
        if ((tempNode.row[i].hash) == hash) {
            unlockTranspositionEntryWhite(upToDownHash);
            return createSymmetricalUpToDownMove(tempNode.row[i].nextMove);
        }
    }
    unlockTranspositionEntryWhite(upToDownHash);

    //simmetria down to up
    lockTranspositionEntryWhite(downToUpHash);
    tempNode = WHITE_TABLE[downToUpHash % TRANSPOSITION_TABLE_DIM];
    for (i = 0; i < tempNode.dim; i++) {
        if ((tempNode.row[i].hash) == hash) {
            unlockTranspositionEntryWhite(downToUpHash);
            return createSymmetricalDownToUpMove(tempNode.row[i].nextMove);
        }
    }
    unlockTranspositionEntryWhite(downToUpHash);

    //simmetria 90 deegres
    lockTranspositionEntryWhite(degrees90Hash);
    tempNode = WHITE_TABLE[degrees90Hash % TRANSPOSITION_TABLE_DIM];
    for (i = 0; i < tempNode.dim; i++) {
        if ((tempNode.row[i].hash) == hash) {
            unlockTranspositionEntryWhite(degrees90Hash);
            return create90degreesMove(create90degreesMove(create90degreesMove(tempNode.row[i].nextMove)));
        }
    }
    unlockTranspositionEntryWhite(degrees90Hash);

    //180 gradi
    lockTranspositionEntryWhite(degrees180Hash);
    tempNode = WHITE_TABLE[degrees180Hash % TRANSPOSITION_TABLE_DIM];
    for (i = 0; i < tempNode.dim; i++) {
        if ((tempNode.row[i].hash) == hash) {
            unlockTranspositionEntryWhite(degrees180Hash);
            return create90degreesMove(create90degreesMove(tempNode.row[i].nextMove));
        }
    }
    unlockTranspositionEntryWhite(degrees180Hash);

    //270 gradi
    lockTranspositionEntryWhite(degrees270Hash);
    tempNode = WHITE_TABLE[degrees270Hash % TRANSPOSITION_TABLE_DIM];
    for (i = 0; i < tempNode.dim; i++) {
        if ((tempNode.row[i].hash) == hash) {
            unlockTranspositionEntryWhite(degrees270Hash);
            return create90degreesMove(tempNode.row[i].nextMove);
        }
    }
    unlockTranspositionEntryWhite(degrees270Hash);

    printf("MOSSA NON TROVATA\n");

    return MOVE_NOT_FOUND;
}


inline void lockTranspositionEntryWhite(uint64_t hash) {
    pthread_spin_lock(&(TABLE_LOCK_WHITE[hash % TRANSPOSITION_TABLE_DIM]));
}

inline void  unlockTranspositionEntryWhite(uint64_t hash) {
    pthread_spin_unlock(&(TABLE_LOCK_WHITE[hash % TRANSPOSITION_TABLE_DIM]));
}

inline void lockTranspositionEntryBlack(uint64_t hash) {
    pthread_spin_lock(&(TABLE_LOCK_BLACK[hash % TRANSPOSITION_TABLE_DIM]));
}

inline void unlockTranspositionEntryBlack(uint64_t hash) {
    pthread_spin_unlock(&(TABLE_LOCK_BLACK[hash % TRANSPOSITION_TABLE_DIM]));
}
