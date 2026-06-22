#include <graphics.h>
#include <time.h>
#include <stdio.h>
#include "../include/clock.h"

void drawClock()
{
    time_t now = time(NULL);
    tm *t = localtime(&now);

    char buffer[10];

    sprintf(
        buffer,
        "%02d:%02d",
        t->tm_hour,
        t->tm_min
    );

    setcolor(WHITE);

    outtextxy(
        getmaxx() - 70,
        getmaxy() - 35,
        buffer
    );
}