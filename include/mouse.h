/*
 * ============================================================
 * mouse.h — Deklarasi modul Input (Mouse & Keyboard)
 * ============================================================
 * Modul ini menangani semua interaksi pengguna:
 *
 * Mouse (handleMouse):
 *  - Klik tombol Start → toggle Start Menu (buka/tutup)
 *  - Klik "Turn Off Computer" → keluar program
 *  - Klik di luar area menu → tutup Start Menu
 *
 * Keyboard (handleKeyboard):
 *  - Tombol Windows (VK_LWIN / VK_RWIN) → toggle Start Menu
 *
 * Kedua fungsi mengembalikan bool:
 *  true  = ada aksi, main loop perlu redraw
 *  false = tidak ada aksi, main loop bisa skip redraw
 * ============================================================
 */

#ifndef MOUSE_H
#define MOUSE_H

// Memproses klik kiri mouse: toggle menu, keluar program, tutup menu
// Return true jika ada aksi yang memerlukan redraw
bool handleMouse();

// Memproses input keyboard: tombol Windows → toggle menu
// Return true jika ada aksi yang memerlukan redraw
bool handleKeyboard();

#endif