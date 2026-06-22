#include <graphics.h>
#include "../include/mouse.h"
#include "../include/startmenu.h"

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