#include <graphics.h>
#include <math.h>
#include "../include/startmenu.h"
#include "../include/utils.h"

static float animProgress = 0.0f;
static float popupAnimProgress = 0.0f;
static bool isHoveringPrograms = false;

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
    int menuWidth = 350;

    int leftColWidth = 200;
    int rightColWidth = menuWidth - leftColWidth;
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
        outtextxy(menuX + 60, menuY + 20, "Administrator");
        
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
    if (animProgress > 0.8f) {
        // Teks Footer
        setbkcolor(BLUE);
        setcolor(WHITE);
        outtextxy(menuX + 20, menuY + currentHeight - 30, "Log Off");
        outtextxy(menuX + 180, menuY + currentHeight - 30, "Turn Off Computer");

        // Menu Kiri
        setbkcolor(WHITE); // Samakan background teks dengan kotak putih
        setcolor(BLACK);
        outtextxy(menuX + 15, menuY + headerHeight + 20, "Internet Explorer");
        outtextxy(menuX + 15, menuY + headerHeight + 50, "Outlook Express");
        outtextxy(menuX + 15, menuY + headerHeight + 80, "Windows Media Player");
        outtextxy(menuX + 15, menuY + headerHeight + 110, "MSN Explorer");
        outtextxy(menuX + 15, menuY + headerHeight + 140, "Windows Movie Maker");

        // Pembatas
        setcolor(LIGHTGRAY);
        line(menuX + 10, menuY + headerHeight + 170, menuX + leftColWidth - 10, menuY + headerHeight + 170);

        // Tombol All Programs
        int allProgY = menuY + currentHeight - footerHeight - 30;
        setcolor(BLACK);
        outtextxy(menuX + 15, allProgY, "All Programs     >");

        // Deteksi hover pada tombol "All Programs" atau pop-up menu
        if (mx >= menuX && mx <= menuX + menuWidth + 200 && my >= allProgY - 20 && my <= startY) {
            isHoveringPrograms = true;
        } else {
            isHoveringPrograms = false;
        }

        // Menu Kanan
        setbkcolor(LIGHTBLUE); // Samakan background teks dengan kotak biru muda
        setcolor(WHITE);
        outtextxy(menuX + leftColWidth + 15, menuY + headerHeight + 20, "My Documents");
        outtextxy(menuX + leftColWidth + 15, menuY + headerHeight + 50, "My Recent Documents");
        outtextxy(menuX + leftColWidth + 15, menuY + headerHeight + 80, "My Pictures");
        outtextxy(menuX + leftColWidth + 15, menuY + headerHeight + 110, "My Music");
        outtextxy(menuX + leftColWidth + 15, menuY + headerHeight + 140, "My Computer");
        
        outtextxy(menuX + leftColWidth + 15, menuY + headerHeight + 180, "Control Panel");
        outtextxy(menuX + leftColWidth + 15, menuY + headerHeight + 210, "Connect To");
        outtextxy(menuX + leftColWidth + 15, menuY + headerHeight + 240, "Printers and Faxes");
    }

    setbkcolor(oldBk); // Kembalikan ke background awal

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
        int popupWidth = 180;
        int popupTargetHeight = 250;
        int popupHeight = (int)(popupTargetHeight * popupAnimProgress);
        int popupX = menuX + leftColWidth; // Muncul dari batas kolom kiri
        int popupY = menuY + currentHeight - footerHeight - popupTargetHeight;

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
            setbkcolor(WHITE); // Set background teks menjadi putih untuk pop-up
            setcolor(BLACK);
            settextstyle(SANS_SERIF_FONT, HORIZ_DIR, 1);
            outtextxy(popupX + 15, popupY + 20, "Accessories");
            outtextxy(popupX + 15, popupY + 50, "Games");
            outtextxy(popupX + 15, popupY + 80, "Startup");
            outtextxy(popupX + 15, popupY + 110, "Internet Explorer");
            outtextxy(popupX + 15, popupY + 140, "MSN");
            outtextxy(popupX + 15, popupY + 170, "Outlook Express");
            outtextxy(popupX + 15, popupY + 200, "Remote Assistance");
            outtextxy(popupX + 15, popupY + 230, "Windows Media Player");
            setbkcolor(oldBk); // Kembalikan
        }
    }
}

void toggleStartMenu()
{
    startMenuOpen = !startMenuOpen;
}