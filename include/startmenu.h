/*
 * ============================================================
 * startmenu.h — Deklarasi modul Start Menu
 * ============================================================
 * Modul paling kompleks dalam proyek. Menangani:
 *  - Animasi buka/tutup menu dengan linear interpolation (0.0 → 1.0)
 *  - Layout dua kolom: kiri (putih, menu utama), kanan (biru muda)
 *  - Header "Administrator" + avatar placeholder
 *  - Footer dengan tombol "Log Off" & "Turn Off Computer"
 *  - 15+ item menu dengan deteksi hover (highlight saat mouse di atas)
 *  - Pop-up sub-menu "All Programs" dengan animasi dan shadow
 *
 * getAnimProgress() digunakan main loop untuk mendeteksi
 * apakah animasi sedang berjalan (perlu redraw terus-menerus).
 *
 * isHoveringProgramsChanged() digunakan main loop untuk
 * mendeteksi perubahan hover pada pop-up menu.
 * ============================================================
 */

#ifndef STARTMENU_H
#define STARTMENU_H

// Menggambar Start Menu lengkap (header, 2 kolom, footer, item, pop-up)
void drawStartMenu();

// Toggle Start Menu: buka jika tertutup, tutup jika terbuka
void toggleStartMenu();

// Mengembalikan progress animasi saat ini (0.0 = tersembunyi, 1.0 = terbuka penuh)
// Digunakan main loop untuk mendeteksi apakah animasi sedang berjalan
float getAnimProgress();

// Mengecek apakah hover state pada tombol "All Programs" berubah
// Return true jika mouse bergerak atau hover state berubah
bool isHoveringProgramsChanged();

#endif