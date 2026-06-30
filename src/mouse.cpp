/*
 * ============================================================
 * mouse.cpp — Implementasi modul Input (Mouse & Keyboard)
 * ============================================================
 * Menangani semua interaksi pengguna dengan program.
 *
 * Mouse (handleMouse):
 *  - Klik tombol Start → toggle Start Menu (buka/tutup)
 *  - Klik "Turn Off Computer" → set isRunning = false (keluar)
 *  - Klik di luar area menu saat menu terbuka → tutup menu
 *
 * Keyboard (handleKeyboard):
 *  - Tombol Windows (VK_LWIN / VK_RWIN) → toggle Start Menu
 *
 * Kedua fungsi mengembalikan bool:
 *  true  = ada aksi → main loop perlu redraw layar
 *  false = tidak ada aksi → main loop bisa skip redraw
 *
 * CATATAN: Koordinat bounding box tombol Start dan item menu
 * di-hardcode. Jika posisi di startmenu.cpp berubah, koordinat
 * di sini juga harus diupdate manual (masalah coupling).
 * ============================================================
 */

#include <graphics.h>
#include "../include/mouse.h"
#include "../include/startmenu.h"  // toggleStartMenu()
#include "../include/utils.h"      // startMenuOpen, isRunning
#include <stdlib.h>                // (mungkin tidak terpakai di versi ini)

bool handleMouse()
{
    // Cek apakah ada event klik kiri dalam antrean mouse
    // WM_LBUTTONDOWN = Left Button Down (klik kiri)
    if (!ismouseclick(WM_LBUTTONDOWN))
        return false;  // Tidak ada klik → tidak ada yang perlu dilakukan

    // Ambil koordinat klik DAN hapus event dari antrean
    // (jika tidak dihapus, event yang sama akan diproses lagi di frame berikutnya)
    int x, y;
    getmouseclick(WM_LBUTTONDOWN, x, y);

    // --- Cek 1: Klik tombol Start? ---
    // Area tombol Start: X=10-80, Y=getmaxy()-40 sampai getmaxy()-10
    // (area klik sedikit lebih besar dari ikon untuk memudahkan user)
    if (x >= 10 && x <= 80 &&
        y >= getmaxy() - 40 &&
        y <= getmaxy() - 10)
    {
        toggleStartMenu();  // Buka jika tertutup, tutup jika terbuka
        return true;        // State berubah → perlu redraw
    }

    // --- Jika menu sedang terbuka, cek klik pada item menu ---
    if (startMenuOpen)
    {
        // Posisi Y taskbar dan menu (harus sinkron dengan startmenu.cpp)
        int startY = getmaxy() - 50;   // Y taskbar
        int menuY = startY - 450;      // Y menu saat terbuka penuh

        // --- Cek 2: Klik "Turn Off Computer"? ---
        // Area tombol: X=250-400, Y=menuY+410 sampai menuY+440
        if (x >= 250 &&
            x <= 400 &&
            y >= menuY + 410 &&
            y <= menuY + 440)
        {
            isRunning = false;  // Set flag → main loop akan berhenti
            return true;        // Perlu redraw (sebelum keluar)
        }

        // --- Cek 3: Klik di luar area menu? ---
        // Jika user klik di luar kotak menu (kiri, kanan, atas, bawah),
        // tutup menu (seperti perilaku Windows XP)
        if (x < 0 ||        // Di kiri menu
            x > 480 ||      // Di kanan menu (menuWidth = 480)
            y < menuY ||    // Di atas menu
            y > startY)     // Di bawah menu (area taskbar di luar tombol Start)
        {
            startMenuOpen = false;  // Tutup menu
            return true;            // Perlu redraw
        }
    }

    return false;  // Klik tidak mengenai area yang ditangani
}

bool handleKeyboard()
{
    // kbhit() = "keyboard hit": cek apakah ada tombol dalam buffer
    // Non-blocking: langsung return, tidak menunggu user menekan tombol
    if(kbhit())
    {
        int key = getch();  // Baca kode tombol dari buffer

        // VK_LWIN = Left Windows key, VK_RWIN = Right Windows key
        // Virtual Key codes — konstanta dari graphics.h / windows.h
        if(key == VK_LWIN || key == VK_RWIN)
        {
            toggleStartMenu();  // Toggle seperti klik tombol Start
            return true;        // Perlu redraw
        }
    }
    return false;  // Tidak ada tombol Windows yang ditekan
}