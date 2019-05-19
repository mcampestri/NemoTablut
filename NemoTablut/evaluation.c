#include "evaluation.h"
#include "moves.h"

#include <string.h>


int isWhiteWin(BoardState board) {
    return
            ((board.Board[1] & 0b110000000000000000U) == 0b110000000000000000U || ((board.Board[1] & 0b000000000000000011U) == 0b000000000000000011U)) ||
            ((board.Board[2] & 0b110000000000000000U) == 0b110000000000000000U || ((board.Board[2] & 0b000000000000000011U) == 0b000000000000000011U)) ||

            ((board.Board[6] & 0b110000000000000000U) == 0b110000000000000000U || ((board.Board[6] & 0b000000000000000011U) == 0b000000000000000011U)) ||
            ((board.Board[7] & 0b110000000000000000U) == 0b110000000000000000U || ((board.Board[7] & 0b000000000000000011U) == 0b000000000000000011U)) ||

            ((board.Board[0] & 0b001100000000000000U) == 0b001100000000000000U || ((board.Board[0] & 0b000011000000000000U) == 0b000011000000000000U)) ||
            ((board.Board[0] & 0b000000000000110000U) == 0b000000000000110000U || ((board.Board[0] & 0b000000000000001100U) == 0b000000000000001100U)) ||

            ((board.Board[8] & 0b001100000000000000U) == 0b001100000000000000U || ((board.Board[8] & 0b000011000000000000U) == 0b000011000000000000U)) ||
            ((board.Board[8] & 0b000000000000110000U) == 0b000000000000110000U || ((board.Board[8] & 0b000000000000001100U) == 0b000000000000001100U));
}


//--------------------------------------------------------------------------------------

/*
ScoreType evaluateGioele(BoardState board) {
	int i, j;
    int kingDistMin, kingDistUp, kingDistDown, kingDistSx, kingDistDx, kingDistanceModifier, throneDist;
    int kingDefendedModifier = 0;
    int found = 0;

	if (isWhiteWin(board)) {
		return WHITE_WIN;
	}
 */
/*
if (isBlackWin(board)) {
    return BLACK_WIN;
}
*/
/*
int numWhite = 0;
int numBlack = 0;
unsigned int cell;
int whiteKingFound = 0;
for (i = 0; i < 9; i++) {
    for (j = 0; j < 9; j++) {
        cell = GET_CELL_STATE(board.Board, i, j);
        if (cell > 0) {
            whiteKingFound += (cell == WHITE_KING);
            numWhite += (cell >> 1);
            numBlack += (cell & 1U);
        }
    }
}
if (!whiteKingFound)
    return BLACK_WIN;
// tra -9 e +17 (*1.5) (*10) (forse dovrei dare più peso al num pezzi -> *2 ?)
int numPiecesModifier = ((numWhite - 9) - (numBlack - 17)) * 1.5 * 10;


for (i = 0; i < 9 && !found; i++) {
    for (j = 0; j < 9 && !found; j++) {
        if (GET_CELL_STATE(board.Board, i, j) == WHITE_KING) {
            // kingDistanceModifier, manhattan distances da ognuna delle caselle centrali della riga/colonna -> ok o rispetto ad un'altra casella?
            kingDistUp = abs(i - 0) + abs(j - 4);
            kingDistDown = abs(i - 8) + abs(j - 4);
            kingDistSx = abs(i - 4) + abs(j - 0);
            kingDistDx = abs(i - 4) + abs(j - 4);

            // kingDefendedModifier
            if (GET_CELL_STATE(board.Board, i - 1, j) == 2) // defending up
                kingDefendedModifier++;
            if (GET_CELL_STATE(board.Board, i + 1, j) == 2) // defending down
                kingDefendedModifier++;
            if (GET_CELL_STATE(board.Board, i, j - 1) == 2) // defending sx
                kingDefendedModifier++;
            if (GET_CELL_STATE(board.Board, i, j + 1) == 2) // defending dx
                kingDefendedModifier++;
            // manhattan distance
            throneDist = abs(i - 4) + abs(j - 4);

            found++;
        }
    }
}
kingDistMin = min(min(min(kingDistUp, kingDistDown), kingDistSx), kingDistDx);
// tra 0 e 1 (*10)
kingDistanceModifier = (10 - kingDistMin) *//* * 0.1*//*; // 10 è distanza massima possibile dalle caselle scelte al momento
	// tra 0 e 0.6 (*10)
	kingDefendedModifier = (kingDefendedModifier / 4.0) * (throneDist < 2? throneDist * 0.2 : 0.6) * 10; // peso dipendente dalla distanza dal trono, voglio evitare re che non si muove da lì

	float emptyRowColumnModifier = 0;
	BoardState rotated90Board;
	Create90degrees(&board, &rotated90Board);
	// caso in cui considero tutte le righe e colonne
	for (i = 0;i < 9;i++) {
		if (board.Board[i] == 0)
			emptyRowColumnModifier += (i == 2 || i == 6) ? 3 : ((i == 1 || i == 7) ? 2 : 0.5); // molto più peso alle righe 2 e 6, più peso alle righe 1 e 7, più probabili per la vittoria
		if (rotated90Board.Board[i] == 0)
			emptyRowColumnModifier += (i == 2 || i == 6) ? 3 : ((i == 1 || i == 7) ? 2 : 0.5); // molto più peso alle colonne 2 e 6, più peso alle colonne 1 e 7, più probabili per la vittoria
	}
	// tra 0 e 1.25 (1.25 non realmente raggiungibile) (*10)
	emptyRowColumnModifier = (emptyRowColumnModifier / 20.0) * 10; // 20 non è la somma max ma è improbabile che si raggiunga un numero tale di righe e colonne vuote che il modificatore vada sopra all'1.0
	*/
/*
Oppure caso in cui considero solo le righe e colonne importanti
if(board.Board[1] == 0)
    emptyRowColumnModifier++;
if(board.Board[2] == 0)
    emptyRowColumnModifier += 2;
if(board.Board[6] == 0)
    emptyRowColumnModifier += 2;
if(board.Board[7] == 0)
    emptyRowColumnModifier++;
if(rotated90Board.Board[1] == 0)
    emptyRowColumnModifier++;
if(rotated90Board.Board[2] == 0)
    emptyRowColumnModifier += 2;
if(rotated90Board.Board[6] == 0)
    emptyRowColumnModifier += 2;
if(rotated90Board.Board[7] == 0)
    emptyRowColumnModifier++;
// tra 0 e 1 (*10)
emptyRowColumnModifier = (emptyRowColumnModifier / 12.0) * 10;
*/

/* Actually non mi convince granché
int diagonalFormationModifier = 0;
for (i = 0;i < 9;i++) {
    for (j = 0;j < 9;j++) {
        if (GET_CELL_STATE(board.Board, i, j) > 1 && i > 0 && j > 0 && i < 8 && j < 8) { // se c'è un bianco o un re e sono in una zona in cui è possibile avere una diagonale
            if ((GET_CELL_STATE(board.Board, i - 1, j - 1) > 1 && GET_CELL_STATE(board.Board, i + 1, j + 1) > 1) ||
                (GET_CELL_STATE(board.Board, i + 1, j - 1) > 1 && GET_CELL_STATE(board.Board, i - 1, j + 1) > 1)) // almeno tre in diagonale
                diagonalFormationModifier++;
        }
    }
}
// tra 0 e 0.7 (*10)
diagonalFormationModifier *= 0.1 * 10; // possibile che sia da rendere meno influente, formazioni diagonali utili soprattutto in difesa per i neri
*/
/*
int importantSquaresModifier = 0;
BoardState temp, resultBoard;
memcpy(&temp, &board, 36);
for (i = 0;i < 4;i++) {
    if (GET_CELL_STATE(temp.Board, 0, 2) == 1)
        importantSquaresModifier++;
    if (GET_CELL_STATE(temp.Board, 1, 2) == 1)
        importantSquaresModifier++;
    if (GET_CELL_STATE(temp.Board, 1, 1) == 1)
        importantSquaresModifier++;
    if (GET_CELL_STATE(temp.Board, 2, 1) == 1)
        importantSquaresModifier++;
    if (GET_CELL_STATE(temp.Board, 2, 0) == 1)
        importantSquaresModifier++;
    Create90degrees(&temp, &resultBoard);
    temp = resultBoard;
}
// tra -1 e 0 (*10)
importantSquaresModifier = - ((importantSquaresModifier / 20.0) * 10); // modifier negativo!

// int distributedControlModifier = 0; // avere almeno un pezzo per riga o per colonna è nice ma sarebbe più una cosa da evaluateBlack
// int piecesDevelopmentModifier = 0; // per evitare di muovere sempre gli stessi pezzi dovrei avere la mossa effettuata come parametro

// NB se inserisco l'altro emptyRowColumnModifier il conto e la normalizzazione vanno rifatti!
// tra -14.5 e +28.35 (*10) -> tra -145 e +283 -> normalizzo su 90 max
int result = numPiecesModifier + kingDistanceModifier + kingDefendedModifier + emptyRowColumnModifier *//*+ diagonalFormationModifier*//* + importantSquaresModifier;
	result = (result / 283.0) * 90;

	return result;
}
*/




/*
#define BLACK_PAWN_VALUE -5	//valore aggiunto per ogni pedone nero
#define WHITE_PAWN_VALUE 5	//valore aggiunto per ogni pedone nero
#define LINE_VALUE -2		 //valore aggiunto se ti trovi nelle linee di vittoria del bianco
#define NEXT_KING -2		//valore se il nero si trova vicino al re
*/




/*
// Euristiche un po' più complesse

#define BLACK_PAWN_VALUE -3	//valore aggiunto per ogni pedone nero							
#define WHITE_PAWN_VALUE 4	//valore aggiunto per ogni pedone nero							
#define LINE_VALUE -1		 //valore aggiunto se ti trovi nelle linee di vittoria del bianco-1
#define NEXT_KING -2		//valore se il nero si trova vicino al re

#define START_VALUE_FOR_WHITE 12
#define START_VALUE_FOR_BLACK 12
#define CAMP_LINE -1
#define NEXT_PAWN 1

//chiamata da min. ha appena mosso il bianco
ScoreType evaluateForWhite(BoardState board) {
    int result = START_VALUE_FOR_WHITE;
    uint32_t i, j;
    uint32_t cell;
    uint32_t  lastCell = 0;
    uint32_t lastCellInColumn[9] = { 0, 0, 0, 0, 0, 0, 0, 0, 0 };


    for (i = 0; i < 9; i++) {
        for (j = 0; j < 9; j++) {
            cell = GET_CELL_STATE(board.Board, i, j);
            switch (cell) {
                //--------------MODIFICATORI PER IL NERO-------------------------
                case BLACK:
                    if (i == 2 || i == 6)
                        result += LINE_VALUE;
                    if(i==1 || i == 7)
                        result += CAMP_LINE;
                    if (j == 2 || j == 6)
                        result += LINE_VALUE;
                    if (j == 1 || j == 7)
                        result += CAMP_LINE;
                    result += BLACK_PAWN_VALUE;
                    if (lastCell == BLACK)
                        result += NEXT_PAWN;
                    if(lastCellInColumn[j] == BLACK)
                        result += NEXT_PAWN;
                    break;
                    //----------------------MODIFICATORI PER IL BIANCO---------------------------
                case WHITE:
                    result += WHITE_PAWN_VALUE;
                    if (lastCell == WHITE)
                        result -= NEXT_PAWN;
                    if (lastCellInColumn[j] == WHITE)
                        result -= NEXT_PAWN;
                    break;
                    //-------------------MODIFICATORI PER IL RE------------------------------------
                case WHITE_KING:
                    if (i == 0 || i == 8 || j == 0 || j == 8)
                        return WHITE_WIN;
                    result +=
                            (GET_CELL_STATE(board.Board, i - 1, j) == BLACK) * NEXT_KING +
                            (GET_CELL_STATE(board.Board, i, j - 1) == BLACK) * NEXT_KING +
                            (GET_CELL_STATE(board.Board, i + 1, j) == BLACK) * NEXT_KING +
                            (GET_CELL_STATE(board.Board, i, j + 1) == BLACK) * NEXT_KING;
                    break;

                default:
                    break;
            }
            lastCellInColumn[j] = cell;
            lastCell = cell;
        }
    }
    return result;
}


//chiamata da max, ha appena mosso il nero
ScoreType evaluateForBlack(BoardState board) {
    int result = START_VALUE_FOR_BLACK;
    uint32_t i, j;
    uint32_t rowKing = 0;
    uint32_t columnKing = 0;
    uint32_t temp = 0;
    uint32_t cell;
    uint32_t lastCell = 0;
    int kingFound = 0;
    int lastCellInColumn[9] = { 0, 0, 0, 0, 0, 0, 0, 0, 0 };


    for (i = 0; i < 9; i++) {
        for (j = 0; j < 9; j++) {
            cell = GET_CELL_STATE(board.Board, i, j);
            switch (cell) {
                //--------------MODIFICATORI PER IL NERO-------------------------
                case BLACK:
                    if (i == 2 || i == 6)
                        result += LINE_VALUE;
                    if (i == 1 || i == 7)
                        result += CAMP_LINE;
                    if (j == 2 || j == 6)
                        result += LINE_VALUE;
                    if (j == 1 || j == 7)
                        result += CAMP_LINE;
                    result += BLACK_PAWN_VALUE;
                    if (lastCell == BLACK)
                        result += NEXT_PAWN;
                    if (lastCellInColumn[j] == BLACK)
                        result += NEXT_PAWN;
                    break;
                    //----------------------MODIFICATORI PER IL BIANCO---------------------------
                case WHITE:
                    result += WHITE_PAWN_VALUE;
                    if (lastCell == WHITE)
                        result -= NEXT_PAWN;
                    if (lastCellInColumn[j] == WHITE)
                        result -= NEXT_PAWN;
                    break;
                    //-------------------MODIFICATORI PER IL RE------------------------------------
                case WHITE_KING:
                    if (i == 0 || i == 8 || j == 0 || j == 8)
                        return WHITE_WIN;
                    kingFound = 1;
                    rowKing = i;
                    columnKing = j;
                    result +=
                            (GET_CELL_STATE(board.Board, i - 1, j) == BLACK) * NEXT_KING +
                            (GET_CELL_STATE(board.Board, i, j - 1) == BLACK) * NEXT_KING +
                            (GET_CELL_STATE(board.Board, i + 1, j) == BLACK) * NEXT_KING +
                            (GET_CELL_STATE(board.Board, i, j + 1) == BLACK) * NEXT_KING;
                    break;
                default: break;
            }

            lastCell = cell;
            lastCellInColumn[j] = cell;
        }
    }

    if (!kingFound)
        return BLACK_WIN;

    //valutazione vittoria anticipata
    if (columnKing == 1 || columnKing == 2 || columnKing == 6 || columnKing == 7) {
        board.Board[4] = board.Board[4] | 0b00000000000000000100000000000100U;

        //--------------------controllo colonna sopra al re vuota--------------------------
        for (i = 0; i < rowKing; i++) {
            temp |= board.Board[i];
        }

        if ((temp >> (2 * (8 - columnKing))) % 4 == 0)
            return ALMOST_WHITE_WIN;
        //------------------controllo colonna sotto al re vuota---------------------------
        temp = 0;
        for (i = rowKing+1; i < 9; i++)
            temp |= board.Board[i];
        if ((temp >> (2 * (8 - columnKing))) % 4 == 0)
            return ALMOST_WHITE_WIN;
    }
    if (rowKing == 1 || rowKing == 2 || rowKing == 6 || rowKing == 7) {
        //------------------------controllo riga a destra vuota---------------------------------
        board.Board[1] = board.Board[2] | 0b00000000000000000000000100000000U;
        board.Board[7] = board.Board[7] | 0b00000000000000000000000100000000U;
        temp= 0b00000000000000111111111111111111U;
        temp = temp >> (columnKing + 1) * 2;
        if ((board.Board[rowKing] & temp) == 0)
            return ALMOST_WHITE_WIN;

        //------------------------controllo riga a sinista vuota---------------------------------
        temp = 0b00000000000000111111111111111111U;
        temp = temp << (9 - columnKing ) * 2;
        if ((board.Board[rowKing] & temp) == 0)
            return ALMOST_WHITE_WIN;
    }
    return result;
}
*/


/*
//-------------------------------------EURISTICA 0.2---------------------------------------------------------------

#define BLACK_PAWN_VALUE (-3)	//valore aggiunto per ogni pedone nero
#define WHITE_PAWN_VALUE 4	//valore aggiunto per ogni pedone nero
#define LINE_VALUE (-3)		 //valore aggiunto se ti trovi nelle linee di vittoria del bianco-1
#define NEXT_KING (-2)		//valore se il nero si trova vicino al re
#define CAMP_LINE (-2)
#define NEXT_PAWN 1


#define WHITE_BONUS 1
#define START_VALUE_FOR_WHITE02 (-14)
#define START_VALUE_FOR_BLACK02 (-14)

//chiamata da min. ha appena mosso il bianco
ScoreType evaluateForWhite(BoardState board) {
    int result = START_VALUE_FOR_WHITE02;
    uint32_t i, j;
    uint32_t cell;
    uint32_t lastCell = 0;
    int lastCellInColumn[9] = { 0, 0, 0, 0, 0, 0, 0, 0, 0 };


    for (i = 0; i < 9; i++) {
        for (j = 0; j < 9; j++) {
            cell = GET_CELL_STATE(board.Board, i, j);
            switch (cell) {
                //--------------MODIFICATORI PER IL NERO-------------------------
                case BLACK:
                    if (i == 2 || i == 6)
                        result += LINE_VALUE;
                    if(i==1 || i == 7)
                        result += CAMP_LINE;
                    if (j == 2 || j == 6)
                        result += LINE_VALUE;
                    if (j == 1 || j == 7)
                        result += CAMP_LINE;
                    result += BLACK_PAWN_VALUE;
                    if (lastCell == BLACK)
                        result += NEXT_PAWN;
                    if(lastCellInColumn[j] == BLACK)
                        result += NEXT_PAWN;
                    break;
                    //----------------------MODIFICATORI PER IL BIANCO---------------------------
                case WHITE:
                    result += WHITE_PAWN_VALUE;
                    if (lastCell == WHITE)
                        result -= NEXT_PAWN;
                    if (lastCellInColumn[j] == WHITE)
                        result -= NEXT_PAWN;
                    break;
                    //-------------------MODIFICATORI PER IL RE------------------------------------
                case WHITE_KING:
                    if (i == 0 || i == 8 || j == 0 || j == 8)
                        return WHITE_WIN;
                    result +=
                            (GET_CELL_STATE(board.Board, i - 1, j) == BLACK) * NEXT_KING +
                            (GET_CELL_STATE(board.Board, i, j - 1) == BLACK) * NEXT_KING +
                            (GET_CELL_STATE(board.Board, i + 1, j) == BLACK) * NEXT_KING +
                            (GET_CELL_STATE(board.Board, i, j + 1) == BLACK) * NEXT_KING;
                    break;

                default:
                    if(i!=3 && i!=4 && i!= 5 && j!=3 && j!= 4 && j!= 5)
                        result+=WHITE_BONUS;
                    break;
            }
            lastCellInColumn[j] = cell;
            lastCell = cell;
        }
    }
    return result;
}

//chiamata da max, ha appena mosso il nero
ScoreType evaluateForBlack(BoardState board) {
    int result = START_VALUE_FOR_BLACK02;
    uint32_t i, j;
    uint32_t rowKing = 0;
    uint32_t columnKing = 0;
    uint32_t temp = 0;
    uint32_t cell;
    uint32_t lastCell = 0;
    int kingFound = 0;
    int lastCellInColumn[9] = { 0, 0, 0, 0, 0, 0, 0, 0, 0 };


    for (i = 0; i < 9; i++) {
        for (j = 0; j < 9; j++) {
            cell = GET_CELL_STATE(board.Board, i, j);
            switch (cell) {
                //--------------MODIFICATORI PER IL NERO-------------------------
                case BLACK:
                    if (i == 2 || i == 6)
                        result += LINE_VALUE;
                    if (i == 1 || i == 7)
                        result += CAMP_LINE;
                    if (j == 2 || j == 6)
                        result += LINE_VALUE;
                    if (j == 1 || j == 7)
                        result += CAMP_LINE;
                    result += BLACK_PAWN_VALUE;
                    if (lastCell == BLACK)
                        result += NEXT_PAWN;
                    if (lastCellInColumn[j] == BLACK)
                        result += NEXT_PAWN;
                    break;
                    //----------------------MODIFICATORI PER IL BIANCO---------------------------
                case WHITE:
                    result += WHITE_PAWN_VALUE;
                    if (lastCell == WHITE)
                        result -= NEXT_PAWN;
                    if (lastCellInColumn[j] == WHITE)
                        result -= NEXT_PAWN;
                    break;
                    //-------------------MODIFICATORI PER IL RE------------------------------------
                case WHITE_KING:
                    if (i == 0 || i == 8 || j == 0 || j == 8)
                        return WHITE_WIN;
                    kingFound = 1;
                    rowKing = i;
                    columnKing = j;
                    result +=
                            (GET_CELL_STATE(board.Board, i - 1, j) == BLACK) * NEXT_KING +
                            (GET_CELL_STATE(board.Board, i, j - 1) == BLACK) * NEXT_KING +
                            (GET_CELL_STATE(board.Board, i + 1, j) == BLACK) * NEXT_KING +
                            (GET_CELL_STATE(board.Board, i, j + 1) == BLACK) * NEXT_KING;
                    break;
                default:
                    if(i!=3 && i!=4 && i!= 5 && j!=3 && j!= 4 && j!= 5)
                        result+=WHITE_BONUS;


                    break;
            }

            lastCell = cell;
            lastCellInColumn[j] = cell;
        }
    }

    if (!kingFound)
        return BLACK_WIN;

    //valutazione vittoria anticipata
    if (columnKing == 1 || columnKing == 2 || columnKing == 6 || columnKing == 7) {
        board.Board[4] = board.Board[4] | 0b00000000000000000100000000000100U;

        //--------------------controllo colonna sopra al re vuota--------------------------
        for (i = 0; i < rowKing; i++) {
            temp |= board.Board[i];
        }

        if ((temp >> (2 * (8 - columnKing))) % 4 == 0)
            return ALMOST_WHITE_WIN;
        //------------------controllo colonna sotto al re vuota---------------------------
        temp = 0;
        for (i = rowKing+1; i < 9; i++)
            temp |= board.Board[i];
        if ((temp >> (2 * (8 - columnKing))) % 4 == 0)
            return ALMOST_WHITE_WIN;
    }
    if (rowKing == 1 || rowKing == 2 || rowKing == 6 || rowKing == 7) {
        //------------------------controllo riga a destra vuota---------------------------------
        board.Board[1] = board.Board[2] | 0b00000000000000000000000100000000U;
        board.Board[7] = board.Board[7] | 0b00000000000000000000000100000000U;
        temp= 0b00000000000000111111111111111111U;
        temp = temp >> (columnKing + 1) * 2;
        if ((board.Board[rowKing] & temp) == 0)
            return ALMOST_WHITE_WIN;

        //------------------------controllo riga a sinista vuota---------------------------------
        temp = 0b00000000000000111111111111111111U;
        temp = temp << (9 - columnKing ) * 2;
        if ((board.Board[rowKing] & temp) == 0)
            return ALMOST_WHITE_WIN;
    }
    return result;
}
*/

/*
//-------------------------------------EURISTICA 0.3---------------------------------------------------------------

#define BLACK_PAWN_VALUE -3	//valore aggiunto per ogni pedone nero
#define WHITE_PAWN_VALUE 4	//valore aggiunto per ogni pedone nero
#define LINE_VALUE -2		 //valore aggiunto se ti trovi nelle linee di vittoria del bianco-1
#define NEXT_KING -2		//valore se il nero si trova vicino al re
#define CAMP_LINE -1
#define NEXT_PAWN 1

#define WHITE_BONUS 1
#define START_VALUE_FOR_WHITE03 -30
#define START_VALUE_FOR_BLACK03 -30
#define DIAGONAL_BONUS 2
//chiamata da min. ha appena mosso il bianco
ScoreType evaluateForWhite(BoardState board) {
    int result = START_VALUE_FOR_WHITE03;
    uint32_t i, j;
    uint32_t cell;
    uint32_t lastCell = 0;
    int lastCellInColumn[9] = { 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    for (i = 0; i < 9; i++) {
        for (j = 0; j < 9; j++) {
            cell = GET_CELL_STATE(board.Board, i, j);
            switch (cell) {
                //--------------MODIFICATORI PER IL NERO-------------------------
                case BLACK:
                    if (i == 2 || i == 6)
                        result += LINE_VALUE;
                    if(i==1 || i == 7)
                        result += CAMP_LINE;
                    if (j == 2 || j == 6)
                        result += LINE_VALUE;
                    if (j == 1 || j == 7)
                        result += CAMP_LINE;
                    result += BLACK_PAWN_VALUE;
                    if (lastCell == BLACK)
                        result += NEXT_PAWN;
                    if(lastCellInColumn[j] == BLACK)
                        result += NEXT_PAWN;
                    break;
                    //----------------------MODIFICATORI PER IL BIANCO---------------------------
                case WHITE:
                    result += WHITE_PAWN_VALUE;
                    if (lastCell == WHITE)
                        result -= NEXT_PAWN;
                    if (lastCellInColumn[j] == WHITE)
                        result -= NEXT_PAWN;
                    if(j>0 && lastCellInColumn[j - 1]==WHITE)
                        result+=DIAGONAL_BONUS;
                    else
                    if(j<8 && lastCellInColumn[j + 1]==WHITE)
                        result+=DIAGONAL_BONUS;

                    break;
                    //-------------------MODIFICATORI PER IL RE------------------------------------
                case WHITE_KING:
                    if (i == 0 || i == 8 || j == 0 || j == 8)
                        return WHITE_WIN;
                    result +=
                            (GET_CELL_STATE(board.Board, i - 1, j) == BLACK) * NEXT_KING +
                            (GET_CELL_STATE(board.Board, i, j - 1) == BLACK) * NEXT_KING +
                            (GET_CELL_STATE(board.Board, i + 1, j) == BLACK) * NEXT_KING +
                            (GET_CELL_STATE(board.Board, i, j + 1) == BLACK) * NEXT_KING;
                    break;

                default:
                    if(i!=3 && i!=4 && i!= 5 && j!=3 && j!= 4 && j!= 5)
                        result+=WHITE_BONUS;
                    break;
            }
            lastCellInColumn[j] = cell;
            lastCell = cell;
        }
    }
    return result;
}

//chiamata da max, ha appena mosso il nero
ScoreType evaluateForBlack(BoardState board) {
    int result = START_VALUE_FOR_BLACK03;
    uint32_t i, j;
    uint32_t rowKing = 0;
    uint32_t columnKing = 0;
    uint32_t temp = 0;
    uint32_t cell;
    uint32_t lastCell = 0;
    int kingFound = 0;
    int lastCellInColumn[9] = { 0, 0, 0, 0, 0, 0, 0, 0, 0 };


    for (i = 0; i < 9; i++) {
        for (j = 0; j < 9; j++) {
            cell = GET_CELL_STATE(board.Board, i, j);
            switch (cell) {
                //--------------MODIFICATORI PER IL NERO-------------------------
                case BLACK:
                    if (i == 2 || i == 6)
                        result += LINE_VALUE;
                    if (i == 1 || i == 7)
                        result += CAMP_LINE;
                    if (j == 2 || j == 6)
                        result += LINE_VALUE;
                    if (j == 1 || j == 7)
                        result += CAMP_LINE;
                    result += BLACK_PAWN_VALUE;
                    if (lastCell == BLACK)
                        result += NEXT_PAWN;
                    if (lastCellInColumn[j] == BLACK)
                        result += NEXT_PAWN;
                    break;
                    //----------------------MODIFICATORI PER IL BIANCO---------------------------
                case WHITE:
                    result += WHITE_PAWN_VALUE;
                    if (lastCell == WHITE)
                        result -= NEXT_PAWN;
                    if (lastCellInColumn[j] == WHITE)
                        result -= NEXT_PAWN;
                    if(j>0 && lastCellInColumn[j - 1]==WHITE)
                        result+=DIAGONAL_BONUS;
                    else
                    if(j<8 && lastCellInColumn[j + 1]==WHITE)
                        result+=DIAGONAL_BONUS;
                    break;
                    //-------------------MODIFICATORI PER IL RE------------------------------------
                case WHITE_KING:
                    if (i == 0 || i == 8 || j == 0 || j == 8)
                        return WHITE_WIN;
                    kingFound = 1;
                    rowKing = i;
                    columnKing = j;
                    result +=
                            (GET_CELL_STATE(board.Board, i - 1, j) == BLACK) * NEXT_KING +
                            (GET_CELL_STATE(board.Board, i, j - 1) == BLACK) * NEXT_KING +
                            (GET_CELL_STATE(board.Board, i + 1, j) == BLACK) * NEXT_KING +
                            (GET_CELL_STATE(board.Board, i, j + 1) == BLACK) * NEXT_KING;
                    break;
                default:
                    if(i!=3 && i!=4 && i!= 5 && j!=3 && j!= 4 && j!= 5)
                        result+=WHITE_BONUS;


                    break;
            }

            lastCell = cell;
            lastCellInColumn[j] = cell;
        }
    }

    if (!kingFound)
        return BLACK_WIN;

    //valutazione vittoria anticipata
    if (columnKing == 1 || columnKing == 2 || columnKing == 6 || columnKing == 7) {
        board.Board[4] = board.Board[4] | 0b00000000000000000100000000000100U;

        //--------------------controllo colonna sopra al re vuota--------------------------
        for (i = 0; i < rowKing; i++) {
            temp |= board.Board[i];
        }

        if ((temp >> (2 * (8 - columnKing))) % 4 == 0)
            return ALMOST_WHITE_WIN;
        //------------------controllo colonna sotto al re vuota---------------------------
        temp = 0;
        for (i = rowKing+1; i < 9; i++)
            temp |= board.Board[i];
        if ((temp >> (2 * (8 - columnKing))) % 4 == 0)
            return ALMOST_WHITE_WIN;
    }
    if (rowKing == 1 || rowKing == 2 || rowKing == 6 || rowKing == 7) {
        //------------------------controllo riga a destra vuota---------------------------------
        board.Board[1] = board.Board[2] | 0b00000000000000000000000100000000U;
        board.Board[7] = board.Board[7] | 0b00000000000000000000000100000000U;
        temp= 0b00000000000000111111111111111111U;
        temp = temp >> (columnKing + 1) * 2;
        if ((board.Board[rowKing] & temp) == 0)
            return ALMOST_WHITE_WIN;

        //------------------------controllo riga a sinista vuota---------------------------------
        temp = 0b00000000000000111111111111111111U;
        temp = temp << (9 - columnKing ) * 2;
        if ((board.Board[rowKing] & temp) == 0)
            return ALMOST_WHITE_WIN;
    }
    return result;
}
*/



//-------------------------------------EURISTICA 0.5---------------------------------------------------------------

//AGGIUNGE 1 PER OGNI RIGA O COLONNA DI DISTANZA TRA LE PEDINE DEL NERO E IL RE

#define BLACK_PAWN_VALUE -4 //valore aggiunto per ogni pedone nero
#define WHITE_PAWN_VALUE 5	//valore aggiunto per ogni pedone nero
#define LINE_VALUE -2	 //valore aggiunto se ti trovi nelle linee di vittoria del bianco-1
#define NEXT_PAWN 1
#define DISTANCE_DIVISOR 4 // più è alto meno la distanza delle pedine del nero da re impatta sul risultato

#define START_VALUE_FOR_WHITE05 3
#define START_VALUE_FOR_BLACK05 3


//chiamata da min. ha appena mosso il bianco
ScoreType evaluateForWhite(BoardState board) {
    int result = START_VALUE_FOR_WHITE05;
    uint32_t i, j;
    uint32_t cell;
    uint32_t  lastCell = 0;
    uint32_t lastCellInColumn[9] = { 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    int kingRow=0;
    int kingColumn=0;
    int rowDistance=0;
    int columnDistance=0;
    int upperEdge[9]={0,0,0,0,0,0,0,0,0};
    int downEdge[9]={0,0,0,0,0,0,0,0,0};
    int leftEdge[9]={0,0,0,0,0,0,0,0,0};
    int rightEdge[9]={0,0,0,0,0,0,0,0,0};
    int whiteBonus=0;
    //primo ciclo per cercare il re
    for (i = 0; i < 9; i++)
        for (j = 0; j < 9; j++)
            if(GET_CELL_STATE(board.Board, i, j)==WHITE_KING){
                if (i == 0 || i == 8 || j == 0 || j == 8)
                    return WHITE_WIN;
                kingRow=i;
                kingColumn=j;
                break;
            }


    for (i = 0; i < 9; i++) {
        for (j = 0; j < 9; j++) {
            cell = GET_CELL_STATE(board.Board, i, j);
            switch (cell) {
                //--------------MODIFICATORI PER IL NERO-------------------------
                case BLACK:
                    result += BLACK_PAWN_VALUE;
                    if(i<3 && j<3){
                        upperEdge[j]=1;
                        leftEdge[i]=1;
                    }
                    if(i<3 && j>5){
                        upperEdge[j]=1;
                        rightEdge[i]=1;
                    }
                    if(i>5 && j<3){
                        downEdge[j]=1;
                        leftEdge[i]=1;
                    }
                    if(i>3 && j>5){
                        downEdge[j]=1;
                        rightEdge[i]=1;
                    }
                    rowDistance=i-kingRow;
                    columnDistance=j-kingColumn;
                    whiteBonus+=rowDistance>0? rowDistance : -rowDistance;
                    whiteBonus+=columnDistance>0? columnDistance : -columnDistance;

                    if (lastCell == BLACK)
                        result += NEXT_PAWN;
                    if(lastCellInColumn[j] == BLACK)
                        result += NEXT_PAWN;

                    break;
                    //----------------------MODIFICATORI PER IL BIANCO---------------------------
                case WHITE:
                    result += WHITE_PAWN_VALUE;
                    if (lastCell == WHITE)
                        result -= NEXT_PAWN;
                    if (lastCellInColumn[j] == WHITE)
                        result -= NEXT_PAWN;
                    break;
                    //-------------------MODIFICATORI PER IL RE------------------------------------
                default:
                    break;
            }
            lastCellInColumn[j] = cell;
            lastCell = cell;
        }
    }
    result+=whiteBonus/DISTANCE_DIVISOR;
    //aggiungo per i bonus excape chiusi
    for(i=1;i<8;i++)
        result+=LINE_VALUE*(upperEdge[i]+downEdge[i]+leftEdge[i]+rightEdge[i]);

    return result;
}

//chiamata da max, ha appena mosso il nero
ScoreType evaluateForBlack(BoardState board) {

    int result = START_VALUE_FOR_BLACK05;
    uint32_t i, j;
    uint32_t cell;
    uint32_t  lastCell = 0;
    uint32_t lastCellInColumn[9] = { 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    int temp;
    int rowKing=0;
    int columnKing=0;
    int rowDistance=0;
    int columnDistance=0;
    int upperEdge[9]={0,0,0,0,0,0,0,0,0};
    int downEdge[9]={0,0,0,0,0,0,0,0,0};
    int leftEdge[9]={0,0,0,0,0,0,0,0,0};
    int rightEdge[9]={0,0,0,0,0,0,0,0,0};
    int whiteBonus=0;
    //primo ciclo per cercare il re
    for (i = 0; i < 9; i++)
        for (j = 0; j < 9; j++)
            if(GET_CELL_STATE(board.Board, i, j)==WHITE_KING){
                rowKing=i;
                columnKing=j;
                break;
            }
    if(rowKing==0 && columnKing==0)
        return BLACK_WIN;

    for (i = 0; i < 9; i++) {
        for (j = 0; j < 9; j++) {
            cell = GET_CELL_STATE(board.Board, i, j);
            switch (cell) {
                //--------------MODIFICATORI PER IL NERO-------------------------
                case BLACK:
                    result += BLACK_PAWN_VALUE;
                    if(i<3 && j<3){
                        upperEdge[j]=1;
                        leftEdge[i]=1;
                    }
                    if(i<3 && j>5){
                        upperEdge[j]=1;
                        rightEdge[i]=1;
                    }
                    if(i>5 && j<3){
                        downEdge[j]=1;
                        leftEdge[i]=1;
                    }
                    if(i>3 && j>5){
                        downEdge[j]=1;
                        rightEdge[i]=1;
                    }
                    rowDistance=i-rowKing;
                    columnDistance=j-columnKing;
                    whiteBonus+=rowDistance>0? rowDistance : -rowDistance;
                    whiteBonus+=columnDistance>0? columnDistance : -columnDistance;
                    if (lastCell == BLACK)
                        result += NEXT_PAWN;
                    if(lastCellInColumn[j] == BLACK)
                        result += NEXT_PAWN;

                    break;
                    //----------------------MODIFICATORI PER IL BIANCO---------------------------
                case WHITE:
                    result += WHITE_PAWN_VALUE;
                    if (lastCell == WHITE)
                        result -= NEXT_PAWN;
                    if (lastCellInColumn[j] == WHITE)
                        result -= NEXT_PAWN;
                    break;
                    //-------------------MODIFICATORI PER IL RE------------------------------------
                default:
                    break;
            }
            lastCellInColumn[j] = cell;
            lastCell = cell;
        }
    }

    result+=whiteBonus/DISTANCE_DIVISOR;


    //aggiungo per i bonus excape chiusi
    for(i=1;i<8;i++)
        result+=LINE_VALUE*(upperEdge[i]+downEdge[i]+leftEdge[i]+rightEdge[i]);


    //valutazione vittoria anticipata
    if (columnKing == 1 || columnKing == 2 || columnKing == 6 || columnKing == 7) {
        board.Board[4] = board.Board[4] | 0b00000000000000000100000000000100U;

        //--------------------controllo colonna sopra al re vuota--------------------------
        for (i = 0; i < rowKing; i++) {
            temp |= board.Board[i];
        }

        if ((temp >> (2 * (8 - columnKing))) % 4 == 0)
            return ALMOST_WHITE_WIN;
        //------------------controllo colonna sotto al re vuota---------------------------
        temp = 0;
        for (i = rowKing+1; i < 9; i++)
            temp |= board.Board[i];
        if ((temp >> (2 * (8 - columnKing))) % 4 == 0)
            return ALMOST_WHITE_WIN;
    }
    if (rowKing == 1 || rowKing == 2 || rowKing == 6 || rowKing == 7) {
        //------------------------controllo riga a destra vuota---------------------------------
        board.Board[1] = board.Board[2] | 0b00000000000000000000000100000000U;
        board.Board[7] = board.Board[7] | 0b00000000000000000000000100000000U;
        temp= 0b00000000000000111111111111111111U;
        temp = temp >> (columnKing + 1) * 2;
        if ((board.Board[rowKing] & temp) == 0)
            return ALMOST_WHITE_WIN;

        //------------------------controllo riga a sinista vuota---------------------------------
        temp = 0b00000000000000111111111111111111U;
        temp = temp << (9 - columnKing ) * 2;
        if ((board.Board[rowKing] & temp) == 0)
            return ALMOST_WHITE_WIN;
    }

    return result;
}
