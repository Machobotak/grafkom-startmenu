#include <iostream>
#include <graphics.h>
#include <stdlib.h> // for malloc, free

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

    // Gambar desktop sekali dan simpan ke buffer agar cepat saat redraw
    cleardevice();
    drawDesktop();
    
    unsigned int bgSize = imagesize(0, 0, getmaxx(), getmaxy());
    void* bgImage = malloc(bgSize);
    if (bgImage) {
        getimage(0, 0, getmaxx(), getmaxy(), bgImage);
    }

    int page = 0;

    // Initial full redraw
    setactivepage(1 - page);
    if (bgImage) {
        putimage(0, 0, bgImage, COPY_PUT);
    } else {
        cleardevice();
        drawDesktop();
    }
    drawTaskbar();
    drawStartMenu();
    setvisualpage(1 - page);
    page = 1 - page;

    while (isRunning)
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
        
        // Dengan double buffering, jika ada bagian yang perlu di-redraw, kita redraw semua
        needsFullRedraw = needsMenuRedraw || needsClockRedraw;
        
        if (needsFullRedraw) {
            setactivepage(1 - page);
            
            // Bersihkan layar dengan me-restore desktop (tanpa panel biru sisa)
            if (bgImage) {
                putimage(0, 0, bgImage, COPY_PUT);
            } else {
                cleardevice();
                drawDesktop();
            }
            
            drawTaskbar();
            drawStartMenu();
            
            setvisualpage(1 - page);
            page = 1 - page;
        }
        
        delay(16); // ~60 FPS
    }

    if (bgImage) {
        free(bgImage);
    }
    closegraph();
    return 0;
}