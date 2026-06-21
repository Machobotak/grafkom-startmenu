#include <graphics.h>
#include "../include/taskbar.h"

void drawTaskbar()
{
    setfillstyle(SOLID_FILL, DARKGRAY);

    bar(
        0,
        getmaxy() - 50,
        getmaxx(),
        getmaxy()
    );

    setcolor(WHITE);
    outtextxy(20, getmaxy() - 35, "Start");
}