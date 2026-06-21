#include <graphics.h>
#include "../include/startmenu.h"
#include "../include/utils.h"

void drawStartMenu()
{
    if(!startMenuOpen)
        return;

    setfillstyle(SOLID_FILL, LIGHTGRAY);

    bar(
        0,
        getmaxy() - 350,
        250,
        getmaxy() - 50
    );

    setcolor(BLACK);

    outtextxy(20, getmaxy() - 320, "Browser");
    outtextxy(20, getmaxy() - 280, "Settings");
    outtextxy(20, getmaxy() - 240, "Profile");
    outtextxy(20, getmaxy() - 200, "Shutdown");
}

void toggleStartMenu()
{
    startMenuOpen = !startMenuOpen;
}