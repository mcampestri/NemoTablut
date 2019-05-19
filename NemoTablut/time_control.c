#include <stdio.h>
#include <unistd.h>

#include "time_control.h"
#include "parameters.h"


int timeOver = 0;


void setTimeOver() {
    timeOver = 1;
}


void resetTimeOver() {
    timeOver = 0;
}


int isTimeOver() {
    return timeOver > 0;
}
