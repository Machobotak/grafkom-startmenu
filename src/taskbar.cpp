/*
 * ============================================================
 * taskbar.cpp — Implementasi modul Taskbar
 * ============================================================
 * Menggambar taskbar di 50 piksel terbawah layar:
 *  - Background abu-abu gelap (DARKGRAY)
 *  - Tombol Start di pojok kiri (area 0-60px)
 *    dengan ikon start.bmp di dalamnya (posisi 10-42px)
 *  - Tiga shortcut aplikasi: Browser, Folder, Settings
 *    masing-masing ikon ~32px dengan jarak ~40px
 *  - Jam digital di pojok kanan (via drawClock())
 *
 * Koordinat taskbarY = getmaxy() - 50
 * Artinya taskbar selalu 50px, tidak peduli resolusi layar.
 * ============================================================
 */

#include <graphics.h>
#include "../include/taskbar.h"
#include "../include/clock.h"  // Untuk drawClock()

void drawTaskbar()
{
    // Posisi Y taskbar: 50 piksel dari bawah layar
    int taskbarY = getmaxy() - 50;

    // --- Background taskbar (memenuhi lebar layar) ---
    setfillstyle(SOLID_FILL, DARKGRAY);
    bar(0, taskbarY, getmaxx(), getmaxy());

    // --- Tombol Start (pojok kiri, lebar 60px) ---
    // Area tombol digambar ulang untuk memastikan background solid
    setfillstyle(SOLID_FILL, DARKGRAY);
    bar(0, taskbarY, 60, getmaxy());

    // Ikon Start di dalam tombol (10px dari kiri, 8px dari atas taskbar)
    readimagefile(
        "assets/start.bmp",
        10,
        taskbarY + 8,
        42,
        taskbarY + 40);

    // =====================
    // Shortcut Taskbar
    // =====================
    // Tiga ikon shortcut di sebelah kanan tombol Start
    // Jarak antar ikon ~40px, dimulai dari X=75

    // Shortcut 1: Browser (X = 75-107)
    readimagefile(
        "assets/browser.bmp",
        75,
        taskbarY + 8,
        107,
        taskbarY + 40);

    // Shortcut 2: Folder (X = 115-147)
    readimagefile(
        "assets/folder.bmp",
        115,
        taskbarY + 8,
        147,
        taskbarY + 40);

    // Shortcut 3: Settings (X = 155-187)
    readimagefile(
        "assets/settings.bmp",
        155,
        taskbarY + 8,
        187,
        taskbarY + 40);

    // --- Jam digital di pojok kanan bawah ---
    // drawClock() menangani format dan right-align
    drawClock();
}