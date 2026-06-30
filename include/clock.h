/*
 * ============================================================
 * clock.h — Deklarasi modul Jam Digital
 * ============================================================
 * Modul ini menampilkan jam dan tanggal di pojok kanan taskbar:
 *  - Format jam:   HH:MM   (contoh: 14:30)
 *  - Format tanggal: DD/MM/YYYY (contoh: 30/06/2026)
 *  - Right-align menggunakan textwidth() agar rapi di kanan
 *
 * isTimeChanged() digunakan main loop untuk optimasi:
 * jam hanya di-redraw ketika menit benar-benar berubah,
 * bukan setiap frame (1 redraw/menit vs 60 redraw/detik).
 * ============================================================
 */

#ifndef CLOCK_H
#define CLOCK_H

// Menggambar jam digital (HH:MM) dan tanggal (DD/MM/YYYY)
void drawClock();

// Mengecek apakah jam/menit berubah sejak pengecekan terakhir
// Return true jika perlu redraw jam
bool isTimeChanged();

#endif