#include <graphics.h>
#include <time.h>
#include <stdio.h>
#include "../include/clock.h"

static int lastDisplayedHour = -1;
static int lastDisplayedMinute = -1;

bool isTimeChanged()
{
    time_t now = time(NULL);
    tm *t = localtime(&now);

    if (t->tm_hour != lastDisplayedHour || t->tm_min != lastDisplayedMinute) {
        lastDisplayedHour = t->tm_hour;
        lastDisplayedMinute = t->tm_min;
        return true;
    }
    return false;
}

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