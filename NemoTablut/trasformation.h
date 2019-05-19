#pragma once


#include <stdio.h>
#include "board.h"
#include "moves.h"
#include "move_type.h"
#include <stdlib.h>
#include <stdint.h>


void CreateSymmetricalVertical(BoardState* board, BoardState* resultBoard);
void CreateSymmetricalHorizontal(BoardState* board, BoardState* resultBoard);
void CreateSymmetricalUpToDown(BoardState* board, BoardState* resultBoard);
void CreateSymmetricalDownToUp(BoardState* board, BoardState* resultBoard);
void Create90degrees(BoardState* board, BoardState* resultBoard);

Move createSymmetricalVerticalMove(Move move);
Move createSymmetricalHorizontalMove(Move move);
Move createSymmetricalUpToDownMove(Move move);
Move createSymmetricalDownToUpMove(Move move);
Move create90degreesMove(Move move);
