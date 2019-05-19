#pragma once
#include"trasformation.h"
#include "board.h"
#include "move_type.h"

#define WHITE_WIN 20000		//valore della vittoria del bianco
#define BLACK_WIN (-20000)		//valore della vittoria del nero
#define ALMOST_WHITE_WIN (WHITE_WIN - 1)	//valore attribuito quando l'euristica vede che il bianco ha quasi vinto

int isWhiteWin(BoardState board);

ScoreType evaluateForWhite(BoardState board);
ScoreType evaluateForBlack(BoardState board);
