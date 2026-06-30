/*
 * ============================================================
 * taskbar.h — Deklarasi modul Taskbar
 * ============================================================
 * Modul ini menggambar taskbar di 50 piksel terbawah layar:
 *  - Background abu-abu gelap (DARKGRAY)
 *  - Tombol Start dengan ikon start.bmp di pojok kiri
 *  - Tiga shortcut ikon: Browser, Folder, Settings
 *  - Memanggil drawClock() untuk menampilkan jam digital
 * ============================================================
 */

#ifndef TASKBAR_H
#define TASKBAR_H

// Menggambar taskbar beserta tombol Start, shortcut, dan jam
void drawTaskbar();

#endif