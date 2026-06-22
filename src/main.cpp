#include <iostream>
#include <graphics.h>

#include "../include/desktop.h"
#include "../include/taskbar.h"
#include "../include/startmenu.h"
#include "../include/mouse.h"

int main()
{
    std::cout << "Program mulai\n";

    initwindow(1280, 720, "Start Menu Clone");

    std::cout << "Window dibuat\n";

    while(true)
    {
        cleardevice();

        handleMouse();
        handleKeyboard();

        drawDesktop();
        drawTaskbar();
        drawStartMenu();

        delay(16);
    }

    return 0;
}