#include <graphics.h>
#include <math.h>
#include "../include/startmenu.h"
#include "../include/utils.h"
#include <stdlib.h>

static float animProgress = 0.0f;
static float popupAnimProgress = 0.0f;
static bool isHoveringPrograms = false;
static bool lastHoveringPrograms = false;
static int lastMouseX = -1;
static int lastMouseY = -1;

static bool drawMenuItem(int x, int y, int w, int h, const char* text, int mx, int my, int bgNormal, int fgNormal, int bgHover, int fgHover, int textX, int textY) {
    bool hovered = (mx >= x && mx <= x + w && my >= y && my <= y + h);
    if (hovered) {
        setfillstyle(SOLID_FILL, bgHover);
        bar(x, y, x + w, y + h);
        setbkcolor(bgHover);
        setcolor(fgHover);
    } else {
        setfillstyle(SOLID_FILL, bgNormal);
        bar(x, y, x + w, y + h);
        setbkcolor(bgNormal);
        setcolor(fgNormal);
    }
    outtextxy(textX, textY, (char*)text);
}

void drawStartMenu()
{
    // Update progress animasi start menu
    float animSpeed = 0.1f;
    if (startMenuOpen) {
        if (animProgress < 1.0f) animProgress += animSpeed;
        if (animProgress > 1.0f) animProgress = 1.0f;
    } else {
        if (animProgress > 0.0f) animProgress -= animSpeed;
        if (animProgress < 0.0f) animProgress = 0.0f;
        isHoveringPrograms = false; // Reset state saat menu tertutup
    }

    if (animProgress <= 0.0f) return;

    int mx = mousex();
    int my = mousey();

    int startY = getmaxy() - 50; // Tinggi taskbar
    int targetHeight = 450;
    int currentHeight = (int)(targetHeight * animProgress);

    int menuY = startY - currentHeight;
    int menuX = 0;
    int menuWidth = 480; // Diperlebar signifikan menjadi 480 agar muat

    int leftColWidth = 220; // Sedikit diperlebar dari 210
    int rightColWidth = menuWidth - leftColWidth; // Menjadi 260
    int headerHeight = 60;
    int footerHeight = 45;

    // Header (Blue - gaya XP)
    setfillstyle(SOLID_FILL, BLUE);
    bar(menuX, menuY, menuX + menuWidth, menuY + headerHeight);
    
    int oldBk = getbkcolor(); // Simpan background lama

    if (animProgress > 0.2f) {
        setbkcolor(BLUE); // Samakan background teks dengan header
        setcolor(WHITE);
        settextstyle(SANS_SERIF_FONT, HORIZ_DIR, 2);
        outtextxy(menuX + 60, menuY + 20, (char*)"Administrator");
        
        // Avatar placeholder
        setfillstyle(SOLID_FILL, LIGHTGRAY);
        bar(menuX + 10, menuY + 10, menuX + 50, menuY + 50);
    }

    // Left Column (White)
    setfillstyle(SOLID_FILL, WHITE);
    bar(menuX, menuY + headerHeight, menuX + leftColWidth, menuY + currentHeight - footerHeight);

    // Right Column (Light Blue)
    setfillstyle(SOLID_FILL, LIGHTBLUE);
    bar(menuX + leftColWidth, menuY + headerHeight, menuX + menuWidth, menuY + currentHeight - footerHeight);

    // Footer (Blue)
    setfillstyle(SOLID_FILL, BLUE);
    bar(menuX, menuY + currentHeight - footerHeight, menuX + menuWidth, menuY + currentHeight);

    settextstyle(SANS_SERIF_FONT, HORIZ_DIR, 1);
    int popupWidth = 180;
    int popupTargetHeight = 250;
    int popupX = menuX + leftColWidth; // Muncul dari batas kolom kiri
    int popupY = menuY + currentHeight - footerHeight - popupTargetHeight;

    bool hoveringPopupArea = false;
    if (popupAnimProgress > 0.0f) {
        // Menggunakan target height agar tidak tertutup saat pop-up baru setengah animasi
        hoveringPopupArea = (mx >= popupX && mx <= popupX + popupWidth && 
                             my >= popupY && my <= popupY + popupTargetHeight);
    }

    bool hoveringButton = false;
    if (animProgress > 0.8f) {
        // Teks Footer
        int footerY = menuY + currentHeight - footerHeight;
        drawMenuItem(menuX + 30, footerY + 5, 80, 30, "Log Off", mx, my, BLUE, WHITE, LIGHTBLUE, WHITE, menuX + 40, menuY + currentHeight - 30);
        if (drawMenuItem(
                menuX + 250,
                footerY + 5,
                150,
                30,
                "Turn Off Computer",
                mx,
                my,
                BLUE,
                WHITE,
                LIGHTBLUE,
                WHITE,
                menuX + 260,
                menuY + currentHeight - 30))
        {
            if (ismouseclick(WM_LBUTTONDOWN))
            {
                clearmouseclick(WM_LBUTTONDOWN);
                closegraph();
                exit(0);
            }
        }
        // Menu Kiri
        int leftX = menuX + 5;
        int leftW = leftColWidth - 10;
        drawMenuItem(leftX, menuY + headerHeight + 15, leftW, 25, "Internet Explorer", mx, my, WHITE, BLACK, BLUE, WHITE, menuX + 15, menuY + headerHeight + 20);
        drawMenuItem(leftX, menuY + headerHeight + 45, leftW, 25, "Outlook Express", mx, my, WHITE, BLACK, BLUE, WHITE, menuX + 15, menuY + headerHeight + 50);
        drawMenuItem(leftX, menuY + headerHeight + 75, leftW, 25, "Windows Media Player", mx, my, WHITE, BLACK, BLUE, WHITE, menuX + 15, menuY + headerHeight + 80);
        drawMenuItem(leftX, menuY + headerHeight + 105, leftW, 25, "MSN Explorer", mx, my, WHITE, BLACK, BLUE, WHITE, menuX + 15, menuY + headerHeight + 110);
        drawMenuItem(leftX, menuY + headerHeight + 135, leftW, 25, "Windows Movie Maker", mx, my, WHITE, BLACK, BLUE, WHITE, menuX + 15, menuY + headerHeight + 140);

        // Pembatas
        setcolor(LIGHTGRAY);
        line(menuX + 10, menuY + headerHeight + 170, menuX + leftColWidth - 10, menuY + headerHeight + 170);

        // Tombol All Programs
        int allProgY = menuY + currentHeight - footerHeight - 30;
        setbkcolor(WHITE);
        setcolor(BLACK);
        outtextxy(menuX + 15, allProgY, (char*)"All Programs     >");

        // Deteksi hover pada tombol "All Programs"
        hoveringButton = (mx >= menuX && mx <= menuX + leftColWidth && 
                          my >= allProgY - 15 && my <= allProgY + 15);

        int rightMx = hoveringPopupArea ? -1 : mx;
        int rightMy = hoveringPopupArea ? -1 : my;

        // Menu Kanan
        int rightX = menuX + leftColWidth + 5;
        int rightW = rightColWidth - 10;
        drawMenuItem(rightX, menuY + headerHeight + 15, rightW, 25, "My Documents", rightMx, rightMy, LIGHTBLUE, WHITE, BLUE, WHITE, menuX + leftColWidth + 15, menuY + headerHeight + 20);
        drawMenuItem(rightX, menuY + headerHeight + 45, rightW, 25, "My Recent Documents", rightMx, rightMy, LIGHTBLUE, WHITE, BLUE, WHITE, menuX + leftColWidth + 15, menuY + headerHeight + 50);
        drawMenuItem(rightX, menuY + headerHeight + 75, rightW, 25, "My Pictures", rightMx, rightMy, LIGHTBLUE, WHITE, BLUE, WHITE, menuX + leftColWidth + 15, menuY + headerHeight + 80);
        drawMenuItem(rightX, menuY + headerHeight + 105, rightW, 25, "My Music", rightMx, rightMy, LIGHTBLUE, WHITE, BLUE, WHITE, menuX + leftColWidth + 15, menuY + headerHeight + 110);
        drawMenuItem(rightX, menuY + headerHeight + 135, rightW, 25, "My Computer", rightMx, rightMy, LIGHTBLUE, WHITE, BLUE, WHITE, menuX + leftColWidth + 15, menuY + headerHeight + 140);
        
        drawMenuItem(rightX, menuY + headerHeight + 175, rightW, 25, "Control Panel", rightMx, rightMy, LIGHTBLUE, WHITE, BLUE, WHITE, menuX + leftColWidth + 15, menuY + headerHeight + 180);
        drawMenuItem(rightX, menuY + headerHeight + 205, rightW, 25, "Connect To", rightMx, rightMy, LIGHTBLUE, WHITE, BLUE, WHITE, menuX + leftColWidth + 15, menuY + headerHeight + 210);
        drawMenuItem(rightX, menuY + headerHeight + 235, rightW, 25, "Printers and Faxes", rightMx, rightMy, LIGHTBLUE, WHITE, BLUE, WHITE, menuX + leftColWidth + 15, menuY + headerHeight + 240);
    }

    setbkcolor(oldBk); // Kembalikan ke background awal

    if (animProgress > 0.8f) {
        isHoveringPrograms = hoveringButton || hoveringPopupArea;
    } else {
        isHoveringPrograms = false;
    }

    // Animasi Pop-up menu (All Programs)
    if (isHoveringPrograms) {
        if (popupAnimProgress < 1.0f) popupAnimProgress += animSpeed * 1.5f; // Pop-up lebih cepat
        if (popupAnimProgress > 1.0f) popupAnimProgress = 1.0f;
    } else {
        if (popupAnimProgress > 0.0f) popupAnimProgress -= animSpeed * 1.5f;
        if (popupAnimProgress < 0.0f) popupAnimProgress = 0.0f;
    }

    // Draw Pop-up menu
    if (popupAnimProgress > 0.0f) {
        int popupHeight = (int)(popupTargetHeight * popupAnimProgress);

        // Shadow sederhana
        setfillstyle(SOLID_FILL, DARKGRAY);
        bar(popupX + 5, popupY + popupTargetHeight - popupHeight + 5, popupX + popupWidth + 5, popupY + popupTargetHeight + 5);

        // Background pop-up
        setfillstyle(SOLID_FILL, WHITE);
        bar(popupX, popupY + popupTargetHeight - popupHeight, popupX + popupWidth, popupY + popupTargetHeight);
        
        // Border
        setcolor(BLUE);
        rectangle(popupX, popupY + popupTargetHeight - popupHeight, popupX + popupWidth, popupY + popupTargetHeight);

        // Isi pop-up menu jika animasi hampir selesai
        if (popupAnimProgress > 0.8f) {
            settextstyle(SANS_SERIF_FONT, HORIZ_DIR, 1);
            int popX = popupX + 5;
            int popW = popupWidth - 10;
            drawMenuItem(popX, popupY + 15, popW, 25, "Accessories", mx, my, WHITE, BLACK, BLUE, WHITE, popupX + 15, popupY + 20);
            drawMenuItem(popX, popupY + 45, popW, 25, "Games", mx, my, WHITE, BLACK, BLUE, WHITE, popupX + 15, popupY + 50);
            drawMenuItem(popX, popupY + 75, popW, 25, "Startup", mx, my, WHITE, BLACK, BLUE, WHITE, popupX + 15, popupY + 80);
            drawMenuItem(popX, popupY + 105, popW, 25, "Internet Explorer", mx, my, WHITE, BLACK, BLUE, WHITE, popupX + 15, popupY + 110);
            drawMenuItem(popX, popupY + 135, popW, 25, "MSN", mx, my, WHITE, BLACK, BLUE, WHITE, popupX + 15, popupY + 140);
            drawMenuItem(popX, popupY + 165, popW, 25, "Outlook Express", mx, my, WHITE, BLACK, BLUE, WHITE, popupX + 15, popupY + 170);
            drawMenuItem(popX, popupY + 195, popW, 25, "Remote Assistance", mx, my, WHITE, BLACK, BLUE, WHITE, popupX + 15, popupY + 200);
            drawMenuItem(popX, popupY + 225, popW, 25, "Windows Media Player", mx, my, WHITE, BLACK, BLUE, WHITE, popupX + 15, popupY + 230);
            setbkcolor(oldBk); // Kembalikan
        }
    }

    // Update last hover state untuk deteksi perubahan (di akhir, setelah semua deteksi)
    lastHoveringPrograms = isHoveringPrograms;
    lastMouseX = mx;
    lastMouseY = my;
}

void toggleStartMenu()
{
    startMenuOpen = !startMenuOpen;
}

float getAnimProgress()
{
    return animProgress;
}

bool isHoveringProgramsChanged()
{
    int mx = mousex();
    int my = mousey();
    
    // Check if mouse moved
    bool mouseMoved = (mx != lastMouseX || my != lastMouseY);
    
    // Check if hover state changed
    bool hoverChanged = (isHoveringPrograms != lastHoveringPrograms);
    
    return mouseMoved || hoverChanged || popupAnimProgress > 0.0f;
}