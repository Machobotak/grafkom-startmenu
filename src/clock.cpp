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

    char timeBuffer[10];
    char dateBuffer[20];

    sprintf(timeBuffer, "%02d:%02d", t->tm_hour, t->tm_min);
    sprintf(dateBuffer, "%02d/%02d/%04d", t->tm_mday, t->tm_mon + 1, t->tm_year + 1900);

    setcolor(WHITE);
    int oldBk = getbkcolor();
    setbkcolor(DARKGRAY);

    int rightMargin = getmaxx() - 15;
    outtextxy(rightMargin - textwidth(timeBuffer), getmaxy() - 38, timeBuffer);
    outtextxy(rightMargin - textwidth(dateBuffer), getmaxy() - 20, dateBuffer);

    setbkcolor(oldBk);
}