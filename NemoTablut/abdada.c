#include "abdada.h"

MovesTableEntry MOVE_TABLE[MOVE_TABLE_DIM];


// Inizializza le entry della table

void initMoveTable() {
    uint64_t i, j;

    for(i = 0; i < MOVE_TABLE_DIM; i++) {
        pthread_spin_init(&(MOVE_TABLE[i].lock), PTHREAD_PROCESS_PRIVATE);
        for(j = 0; j < MOVE_TABLE_WIDTH; j++) {
            MOVE_TABLE[i].searchHashes[j] = 0;
        }
    }
}


// Hash mossa in una posizione
// https://github.com/cimarronOST/Monolith/blob/master/Source/search.cpp (line 500)

inline uint64_t moveHash(Move move, uint64_t boardHash) {
    return boardHash ^ (move * 1664525U + 1013904223U);
}



// Controlla se una mossa in una certa posizione è già cercata da un altro thread
// https://github.com/cimarronOST/Monolith/blob/master/Source/search.cpp (line 516)
//
// returns: 1 se un altro thread sta cercando la mossa
//          0 se nessun thread sta cercando la mossa, in questo caso incrementa il campo threadsSearching

int increaseOrDefer(uint64_t moveHash, int depthLeft) {
    if(depthLeft < DEFER_DEPTH)
        return 0;

    uint64_t i;
    int doDefer = 0;
    uint64_t index = moveHash % MOVE_TABLE_DIM;

    // Acquisizione lock sulla entry
    pthread_spin_lock(&(MOVE_TABLE[index].lock));

    // Se nessun thread sta cercando la mossa inizio la ricerca (incremento threadsSearching)
    for(i = 0; i < MOVE_TABLE_WIDTH; i++) {
        if(MOVE_TABLE[index].searchHashes[i] == moveHash)
            doDefer = 1;
    }

    if(!doDefer) {
        for(i = 0; i < MOVE_TABLE_WIDTH; i++) {
            if(MOVE_TABLE[index].searchHashes[i] == 0) {
                MOVE_TABLE[index].searchHashes[i] = moveHash;
            }
        }
    }

    // Rilascio lock
    pthread_spin_unlock(&(MOVE_TABLE[index].lock));

    return doDefer;
}


void endSearch(uint64_t moveHash, int depthLeft) {
    if(depthLeft < DEFER_DEPTH)
        return;

    uint64_t i;
    uint64_t index = moveHash % MOVE_TABLE_DIM;

    // Acquisizione lock sulla entry
    pthread_spin_lock(&(MOVE_TABLE[index].lock));

    for(i = 0; i < MOVE_TABLE_WIDTH; i++) {
        if(MOVE_TABLE[index].searchHashes[i] == moveHash)
            MOVE_TABLE[index].searchHashes[i] = 0;
    }

    // Rilascio lock
    pthread_spin_unlock(&(MOVE_TABLE[index].lock));
}
