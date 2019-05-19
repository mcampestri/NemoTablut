#include "move_type.h"
#include "client.h"
#include "moves.h"

#include <stdio.h>


void printMove(Move move) {
    char startColumn = columnToServerFormat(START_COLUMN(move));
    char targetColumn = columnToServerFormat(TARGET_COLUMN(move));
    int startRow = rowToServerFormat(START_ROW(move));
    int targetRow = rowToServerFormat(TARGET_ROW(move));

    printf("[%c%d -> %c%d]", startColumn, startRow, targetColumn, targetRow);
}