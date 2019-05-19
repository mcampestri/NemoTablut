#pragma once

#include "parameters.h"
#include "move_type.h"

#include <stdint.h>
#include <pthread.h>


typedef struct {
    uint64_t searchHashes[MOVE_TABLE_WIDTH];   // Numero di thread che stanno cercando la posizione
    pthread_spinlock_t lock;    // Lock per mutua esclusione
} MovesTableEntry;


void initMoveTable();
uint64_t moveHash(Move move, uint64_t boardHash);
int increaseOrDefer(uint64_t moveHash, int depthLeft);
void endSearch(uint64_t moveHash, int depthLeft);
