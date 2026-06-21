#include <graphics.h>
#include "../include/desktop.h"

void drawDesktop()
{
    setfillstyle(SOLID_FILL, BLUE);
    bar(0, 0, getmaxx(), getmaxy());
}