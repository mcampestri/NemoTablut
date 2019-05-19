#include "trasformation.h"


void CreateSymmetricalHorizontal(BoardState* board, BoardState* resultBoard) {
	uint32_t* resBoard = resultBoard->Board;
	
	int i;
	for (i = 0; i < 4; i++) {
		resBoard[i] = board->Board[8 - i];
		resBoard[8 - i] = board->Board[i];
	}
	resBoard[4] = board->Board[4];
}


void CreateSymmetricalVertical(BoardState* board, BoardState* resultBoard) {
	int i, j;
	int n;
	uint32_t bitMask = 0b00000000000000110000000000000000U;
	int casella;
	
	uint32_t* resBoard = resultBoard->Board;

	resBoard[0] = 0U;
	resBoard[1] = 0U;
	resBoard[2] = 0U;
	resBoard[3] = 0U;
	resBoard[4] = 0U;
	resBoard[5] = 0U;
	resBoard[6] = 0U;
	resBoard[7] = 0U;
	resBoard[8] = 0U;


	for (j = 0; j < 9; j++) {
		for (i = 0; i < 18; i += 2) {
			casella = board->Board[j] & (bitMask >> i);

			resBoard[j] = ((n = 16 - 2 * i) > 0) ? resBoard[j] | casella >> n : resBoard[j] | casella << -n;
		}
	}
}




void CreateSymmetricalUpToDown(BoardState* board, BoardState* resultBoard) {
	int i, j;
	int n;
	uint32_t bitMask = 0b00000000000000110000000000000000U;
	int casella;
	
	uint32_t* resBoard = resultBoard->Board;

	resBoard[0] = 0U;
	resBoard[1] = 0U;
	resBoard[2] = 0U;
	resBoard[3] = 0U;
	resBoard[4] = 0U;
	resBoard[5] = 0U;
	resBoard[6] = 0U;
	resBoard[7] = 0U;
	resBoard[8] = 0U;

	for (j = 0; j < 9; j++) {
		for (i = 0; i < 9; i = i + 1) {
			casella = board->Board[i] & (bitMask >> j * 2);

			resBoard[j] = ((n = -2 * j + 2 * i) > 0) ? resBoard[j] | casella >> n : resBoard[j] | casella << -n;
		}
	}
}


void CreateSymmetricalDownToUp(BoardState* board, BoardState* resultBoard) {
	int i, j;
	int n;
	uint32_t bitMask = 0b00000000000000110000000000000000U;
	int casella;
	
	uint32_t* resBoard = resultBoard->Board;
	
	resBoard[0] = 0U;
	resBoard[1] = 0U;
	resBoard[2] = 0U;
	resBoard[3] = 0U;
	resBoard[4] = 0U;
	resBoard[5] = 0U;
	resBoard[6] = 0U;
	resBoard[7] = 0U;
	resBoard[8] = 0U;

	for (j = 0; j < 9; j++) {
		for (i = 0; i < 9; i = i + 1) {
			casella = board->Board[i] & (bitMask >> j * 2);

			resBoard[8 - j] = ((n = 16 - 2 * i - 2 * j) > 0) ? resBoard[8 - j] | casella >> n : resBoard[8 - j] | casella << -n;
		}
	}
}

void Create90degrees(BoardState* board, BoardState* resultBoard) {
	int i, j;
	int n;
	int bitMask = 0b00000000000000110000000000000000U;
	int casella;
	
	uint32_t* resBoard = resultBoard->Board;

	resBoard[0] = 0U;
	resBoard[1] = 0U;
	resBoard[2] = 0U;
	resBoard[3] = 0U;
	resBoard[4] = 0U;
	resBoard[5] = 0U;
	resBoard[6] = 0U;
	resBoard[7] = 0U;
	resBoard[8] = 0U;

	for (j = 0; j < 9; j++) {
		for (i = 0; i < 9; i = i + 1) {
			casella = board->Board[i] & (bitMask >> j * 2);

			resBoard[j] = ((n = 16 - 2 * i - 2 * j) > 0) ? resBoard[j] | casella >> n : resBoard[j] | casella << -n;
		}
	}
}



Move createSymmetricalHorizontalMove(Move move) {
    uint32_t startRow = 8U - START_ROW(move);
    uint32_t startColumn = START_COLUMN(move);
    uint32_t targetRow = 8U - TARGET_ROW(move);
    uint32_t targetColumn = TARGET_COLUMN(move);

	if (move == 0)
		return 0;
		
	return CREATE_MOVE(startRow, startColumn, targetRow, targetColumn);
}

Move createSymmetricalVerticalMove(Move move) {
    uint32_t startRow = START_ROW(move);
    uint32_t startColumn = 8U - START_COLUMN(move);
    uint32_t targetRow = TARGET_ROW(move);
    uint32_t targetColumn = 8U - TARGET_COLUMN(move);

	if (move == 0)
		return 0;
	return CREATE_MOVE(startRow, startColumn, targetRow, targetColumn);
}


Move createSymmetricalUpToDownMove(Move move) {
    uint32_t startRow = START_ROW(move);
    uint32_t startColumn = START_COLUMN(move);
    uint32_t targetRow = TARGET_ROW(move);
    uint32_t targetColumn = TARGET_COLUMN(move);

	if (move == 0)
		return 0;

	return CREATE_MOVE(startColumn, startRow, targetColumn, targetRow);

}



Move createSymmetricalDownToUpMove(Move move) {
	uint32_t startRow = 8U - START_ROW(move);
	uint32_t startColumn = 8U - START_COLUMN(move);
	uint32_t targetRow = 8U - TARGET_ROW(move);
	uint32_t targetColumn = 8U - TARGET_COLUMN(move);

	if (move == 0)
		return 0;

	return CREATE_MOVE(startColumn, startRow, targetColumn, targetRow);


}
Move create90degreesMove(Move move) {
	uint32_t startRow = 8U - START_ROW(move);
	uint32_t startColumn = START_COLUMN(move);
	uint32_t targetRow = 8U - TARGET_ROW(move);
	uint32_t targetColumn = TARGET_COLUMN(move);

	if (move == 0)
		return 0;

	return CREATE_MOVE(startColumn, startRow, targetColumn, targetRow);
}