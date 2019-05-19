#include "search.h"
#include "parameters.h"
#include "search_threads.h"
#include "time_control.h"

#include <pthread.h>
#include <unistd.h>


void iterativeDeepeningSearchWhite(BoardState board, int maxDepth, HistoryArray historyArray, int maxTime, int numThreads) {
	struct timeb start, end;
	int diff;

    resetTimeOver();

	ftime(&start);

	pthread_t searchThreads[numThreads];
	SearchArgs searchArgs[numThreads];

	for(int i = 0; i < numThreads; i++) {
	    searchArgs[i].board = board;
	    searchArgs[i].maxDepth = maxDepth;
	    searchArgs[i].historyArray = historyArray;
	}


	for(int i = 0; i < numThreads; i++) {
        pthread_create(&(searchThreads[i]), NULL, doSearchMax, &(searchArgs[i]));
	}

    printf("\n\nTimer avviato\n\n");
	sleep(maxTime);
	setTimeOver();
    printf("\n\nTimer scaduto\n\n");

    for(int i = 0; i < numThreads; i++) {
        pthread_join(searchThreads[i], NULL);
        ftime(&end);
        diff = (int)(1000.0 * (end.time - start.time) + (end.millitm - start.millitm));
        printf("Joined thread: %d (max depth: %d)\nOperation took %u milliseconds\n\n", i, maxDepth, diff);
    }
}



void iterativeDeepeningSearchBlack(BoardState board, int maxDepth, HistoryArray historyArray, int maxTime, int numThreads) {
    struct timeb start, end;
    int diff;

    resetTimeOver();

    ftime(&start);

    pthread_t searchThreads[numThreads];
    SearchArgs searchArgs[numThreads];

    for(int i = 0; i < numThreads; i++) {
        searchArgs[i].board = board;
        searchArgs[i].maxDepth = maxDepth;
        searchArgs[i].historyArray = historyArray;
    }


    for(int i = 0; i < numThreads; i++) {
        pthread_create(&(searchThreads[i]), NULL, doSearchMin, &(searchArgs[i]));
    }

    printf("\n\nTimer avviato\n\n");
    sleep(maxTime);
    setTimeOver();
    printf("\n\nTimer scaduto\n\n");

    for(int i = 0; i < numThreads; i++) {
        pthread_join(searchThreads[i], NULL);
        ftime(&end);
        diff = (int)(1000.0 * (end.time - start.time) + (end.millitm - start.millitm));
        printf("Joined thread: %d (max depth: %d)\nOperation took %u milliseconds\n\n", i, maxDepth, diff);
    }
}
