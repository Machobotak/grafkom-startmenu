#include <iostream>
#include <graphics.h>

#include "../include/desktop.h"
#include "../include/taskbar.h"
#include "../include/startmenu.h"
#include "../include/mouse.h"
#include "../include/clock.h"
#include "../include/utils.h"

// Flag untuk track apa yang perlu di-redraw
static bool needsFullRedraw = true;
static bool needsMenuRedraw = false;
static bool needsClockRedraw = false;

int main()
{
    std::cout << "Program mulai\n";

    initwindow(1280, 720, "Start Menu Clone");

    std::cout << "Window dibuat\n";

    // Initial full redraw
    cleardevice();
    drawDesktop();
    drawTaskbar();
    drawStartMenu();

    while(true)
    {
        // Deteksi apakah ada perubahan yang perlu di-redraw
        bool mouseAction = handleMouse();
        bool keyAction = handleKeyboard();
        bool timeChanged = isTimeChanged();
        float currentAnimProgress = getAnimProgress();
        
        // Cek apakah ada animasi yang sedang berjalan
        static float lastAnimProgress = 0.0f;
        bool isAnimating = (currentAnimProgress > 0.0f && currentAnimProgress < 1.0f) ||
                          (currentAnimProgress != lastAnimProgress);
        
        // Deteksi ketika animasi selesai (transisi dari animating ke done)
        static bool wasAnimating = false;
        bool animationJustFinished = (wasAnimating && !isAnimating);
        wasAnimating = isAnimating;
        
        lastAnimProgress = currentAnimProgress;
        
        // Cek perubahan hover state dan mouse movement
        bool hoverChanged = isHoveringProgramsChanged();
        
        // Tentukan apa yang perlu di-redraw
        needsMenuRedraw = mouseAction || keyAction || isAnimating || hoverChanged || animationJustFinished;
        needsClockRedraw = timeChanged;
        needsFullRedraw = needsMenuRedraw;
        
        // PARTIAL REDRAW: Hanya clear dan redraw area yang berubah
        if (needsFullRedraw) {
            int menuWidth = 350;
            int targetHeight = 450;
            int taskbarHeight = 50;
            int startY = getmaxy() - taskbarHeight;
            
            // Jika animasi baru saja selesai, clear seluruh area menu untuk cleanup
            if (animationJustFinished) {
                setfillstyle(SOLID_FILL, BLUE);
                bar(0, startY - targetHeight, menuWidth, startY);
            } else {
                // Saat animasi berjalan, hanya clear area yang di-animate
                int currentMenuHeight = (int)(targetHeight * currentAnimProgress);
                int menuY = startY - currentMenuHeight;
                setfillstyle(SOLID_FILL, BLUE);
                bar(0, menuY, menuWidth, startY);
            }
            
            // Clear taskbar area (seluruh width)
            setfillstyle(SOLID_FILL, DARKGRAY);
            bar(0, startY, getmaxx(), getmaxy());
            
            // Redraw hanya bagian yang di-clear
            drawTaskbar();
            drawStartMenu();
        } else if (needsClockRedraw) {
            // Hanya redraw jam di taskbar
            int taskbarY = getmaxy() - 50;
            // Clear area jam saja
            setfillstyle(SOLID_FILL, DARKGRAY);
            bar(getmaxx() - 100, taskbarY, getmaxx(), getmaxy());
            // Redraw jam
            drawClock();
        }
        
        delay(16); // ~60 FPS
    }

    return 0;
}