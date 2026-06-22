#include <graphics.h>
#include "../include/taskbar.h"
#include "../include/clock.h"

void drawTaskbar()
{
    int taskbarY = getmaxy() - 50;

    setfillstyle(SOLID_FILL, DARKGRAY);
    bar(
        0,
        taskbarY,
        getmaxx(),
        getmaxy()
    );

    // Area tombol Start
    setfillstyle(SOLID_FILL, DARKGRAY);
    bar(
        0,
        taskbarY,
        60,
        getmaxy()
    );

    // Gambar logo Start
    readimagefile(
        "assets/start.bmp",
        10,
        taskbarY + 8,
        42,
        taskbarY + 40
    );

    drawClock();
}