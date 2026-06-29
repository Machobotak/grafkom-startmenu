#include <graphics.h>
#include "../include/mouse.h"
#include "../include/startmenu.h"
#include "../include/utils.h"
#include <stdlib.h>

bool handleMouse()
{
    if (!ismouseclick(WM_LBUTTONDOWN))
        return false;

    int x, y;
    getmouseclick(WM_LBUTTONDOWN, x, y);

    // Start Button
    if (x >= 10 && x <= 80 &&
        y >= getmaxy() - 40 &&
        y <= getmaxy() - 10)
    {
        toggleStartMenu();
        return true;
    }

    if (startMenuOpen)
    {
        int startY = getmaxy() - 50;
        int menuY = startY - 450;

        // Turn Off Computer
        if (x >= 250 &&
            x <= 400 &&
            y >= menuY + 410 &&
            y <= menuY + 440)
        {
            isRunning = false;
            return true;
        }

        // Klik luar menu
        if (x < 0 ||
            x > 480 ||
            y < menuY ||
            y > startY)
        {
            startMenuOpen = false;
            return true;
        }
    }

    return false;
}

bool handleKeyboard()
{
    if(kbhit())
    {
        int key = getch();
        // Windows key (left or right)
        if(key == VK_LWIN || key == VK_RWIN)
        {
            toggleStartMenu();
            return true;
        }
    }
    return false;
}