#include <graphics.h>
#include "../include/mouse.h"
#include "../include/startmenu.h"
#include "../include/utils.h"


bool handleMouse()
{
    if(ismouseclick(WM_LBUTTONDOWN))
    {
        int x, y;
        getmouseclick(WM_LBUTTONDOWN, x, y);
        
        // Cek jika klik pada tombol Start di taskbar
        if(x >= 10 && x <= 80 && y >= getmaxy() - 40 && y <= getmaxy() - 10)
        {
            toggleStartMenu();
            return true;
        }
        
        // Jika start menu terbuka, cek apakah klik di luar area menu
        if(startMenuOpen)
        {
            int startY = getmaxy() - 50;  // Tinggi taskbar
            int targetHeight = 450;       // Tinggi menu saat fully open
            int menuY = startY - targetHeight;
            int menuX = 0;
            int menuWidth = 480;
            
            // Cek apakah klik di luar batas-batas menu
            bool isClickOutsideMenu = (x < menuX || x > menuX + menuWidth || 
                                       y < menuY || y > startY);
            
            if(isClickOutsideMenu)
            {
                // Tutup start menu
                startMenuOpen = false;
                return true;
            }
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