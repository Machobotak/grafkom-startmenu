#include <graphics.h>
#include "../include/mouse.h"
#include "../include/startmenu.h"

#define VK_LWIN 91
#define VK_RWIN 92

void handleMouse()
{
    if(ismouseclick(WM_LBUTTONDOWN))
    {
        int x, y;
        getmouseclick(WM_LBUTTONDOWN, x, y);
        if(x >= 10 && x <= 80 && y >= getmaxy() - 40 && y <= getmaxy() - 10)
        {
            toggleStartMenu();
        }
    }
}

void handleKeyboard()
{
    if(kbhit())
    {
        int key = getch();
        // Windows key (left or right)
        if(key == VK_LWIN || key == VK_RWIN)
        {
            toggleStartMenu();
        }
    }
}