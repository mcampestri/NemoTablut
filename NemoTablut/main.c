#include <stdio.h>
#include <sys/resource.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>

#include "traspostition_table.h"
#include "board.h"
#include "search.h"
#include "moves.h"
#include "move_type.h"
#include "evaluation.h"
#include "trasformation.h"
#include "abdada.h"
#include "client.h"




//--------------------------------------MAIN PER TESTARE---------------------------------


int ciao() {
    int maxTime = 15;
    int numThreads = 2;

    BoardState initialBoard;
	initBoard(&initialBoard);

	printf("%d\n", isEqual(initialBoard, initialBoard));

    int turn = 1;
	int end = 0;

	initTables();
	initMoveTable();

	HistoryArray historyArray;
	initHistoryArray(&historyArray);
	
	while(!end) {
	    printf("TURNO: %d\n", turn);

		iterativeDeepeningSearchWhite(initialBoard, 7, historyArray, maxTime, numThreads);
		Move bestMove = getFinalBestMoveWhite();

		printf("Best move: ");
		printMove(bestMove);
		printf("\n");
		initialBoard = moveWhite(initialBoard, bestMove);

		printBoard(initialBoard);

		addPlayedBoard(&historyArray, initialBoard);

		sleep(1);

		end = isWhiteWin(initialBoard);
		if(end) {
		    printf("\n\nWHITE WINS\n\n");
		}


		if(!end) {
            Move blackMoves[MAX_MOVES];
            int movesNumber = getPossibleMovesBlack(initialBoard, blackMoves);
            int random = (133 % turn % movesNumber);
            initialBoard = moveBlack(initialBoard, blackMoves[random]);

            printf("\nMOSSA BLACK: \n");
            printBoard(initialBoard);
            printf("\n---------------------------------------------------\n");

            end = evaluateForBlack(initialBoard) == BLACK_WIN;
            if(end) {
                printf("\n\nBLACK WINS\n\n");
            }

            addPlayedBoard(&historyArray, initialBoard);
        }
		turn++;
	}

	return 0;
}



// argv[1] : colore giocatore
// argv[2] : valore timeout (a questo verranno sottratti 5 secondi di margine)
// argv[3] : numero di thread
int main(int argc, char* argv[]) {
    int playerColor, maxTime, numThreads;
    int i;

    for(i = 0; i < strlen(argv[1]); i++) {
        argv[1][i] = tolower(argv[1][i]);
    }
    printf("%s\n", argv[1]);

    if(strcmp(argv[1], "white") == 0)
        playerColor = WHITE;
    else if(strcmp(argv[1], "black") == 0)
        playerColor = BLACK;
    else
        playerColor = WHITE;


    maxTime = atoi(argv[2]) - 5;        // 5 secondi di margine rispetto al timeout
    printf("Max time: %d\n", maxTime);

    numThreads = atoi(argv[3]);
    printf("Num threads: %d\n\n", numThreads);

    gameLoop(playerColor, maxTime, numThreads);

    return 0;
}
