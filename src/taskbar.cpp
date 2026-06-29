#include <graphics.h>
#include "../include/taskbar.h"
#include "../include/clock.h"

void drawTaskbar()
{
    int taskbarY = getmaxy() - 50;

    // Background taskbar
    setfillstyle(SOLID_FILL, DARKGRAY);
    bar(0, taskbarY, getmaxx(), getmaxy());

    // Tombol Start
    setfillstyle(SOLID_FILL, DARKGRAY);
    bar(0, taskbarY, 60, getmaxy());

    readimagefile(
        "assets/start.bmp",
        10,
        taskbarY + 8,
        42,
        taskbarY + 40);

    // =====================
    // Shortcut Taskbar
    // =====================

    // Browser
    readimagefile(
        "assets/browser.bmp",
        75,
        taskbarY + 8,
        107,
        taskbarY + 40);

    // Folder
    readimagefile(
        "assets/folder.bmp",
        115,
        taskbarY + 8,
        147,
        taskbarY + 40);

    // Settings
    readimagefile(
        "assets/settings.bmp",
        155,
        taskbarY + 8,
        187,
        taskbarY + 40);
        
    // Jam
    drawClock();
}