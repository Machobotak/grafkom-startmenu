#include <graphics.h>
#include "../include/desktop.h"

void drawDesktop()
{
    readimagefile("../assets/wallpaper.bmp",
                  0, 0,
                  getmaxx(), getmaxy());

    setcolor(WHITE);

    readimagefile("../assets/browser.bmp",
                  20, 20,
                  80, 80);
    outtextxy(25, 90, "Browser");

    readimagefile("../assets/folder.bmp",
                  20, 130,
                  80, 190);
    outtextxy(30, 200, "Folder");

    readimagefile("../assets/settings.bmp",
                  20, 240,
                  80, 300);
    outtextxy(25, 310, "Settings");

    readimagefile("../assets/profile.bmp",
                  20, 350,
                  80, 410);
    outtextxy(30, 420, "Profile");

    readimagefile("../assets/power.bmp",
                  20, 460,
                  80, 520);
    outtextxy(35, 530, "Power");
}