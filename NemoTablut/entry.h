#pragma once

#include "move_type.h"

#define PV_NODE 1
#define CUT_NODE 2


typedef int8_t TypeNode;

typedef struct {
    uint64_t hash;                  // 8 byte
    Move nextMove;                  // 2 byte
    ScoreType euristicValue;        // 2 byte
    uint8_t deepLeft;               // 1 byte
    TypeNode type; //1: exact value 2:cut value     // 1 byte
} Entry;


typedef struct {
    Entry row[TRANSPOSITION_TABLE_WIDTH];       // 14 byte * TRANSPOSITION_TABLE_WIDTH
    int8_t dim;                                 // 1 byte
} TraspositionRow;
