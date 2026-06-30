/*
 * ============================================================
 * clock.cpp — Implementasi modul Jam Digital
 * ============================================================
 * Menampilkan waktu dan tanggal di pojok kanan bawah taskbar.
 *
 * Format:
 *  - Jam:     HH:MM       (contoh: 14:30) — baris atas
 *  - Tanggal: DD/MM/YYYY  (contoh: 30/06/2026) — baris bawah
 *
 * Right-align: textwidth() digunakan untuk menghitung lebar teks,
 * lalu dikurangkan dari getmaxx() agar tepi kanan teks rata.
 *
 * Optimasi: isTimeChanged() memungkinkan main loop hanya me-redraw
 * jam ketika menit benar-benar berubah (~1 kali per menit), bukan
 * setiap frame (~60 kali per detik).
 *
 * CATATAN tentang offset waktu C:
 *  - tm_mon  : 0–11 (Januari = 0)  → perlu + 1
 *  - tm_year : tahun sejak 1900     → perlu + 1900
 * ============================================================
 */

#include <graphics.h>
#include <time.h>    // time(), localtime(), struct tm
#include <stdio.h>   // sprintf()
#include "../include/clock.h"

// State: waktu terakhir yang ditampilkan di layar
// Nilai awal -1 memastikan deteksi perubahan pada frame pertama
// (karena jam aktual selalu >= 0, jadi pasti berbeda)
static int lastDisplayedHour = -1;
static int lastDisplayedMinute = -1;

bool isTimeChanged()
{
    // Ambil waktu sistem saat ini
    time_t now = time(NULL);       // Timestamp UNIX (detik sejak 1 Jan 1970)
    tm *t = localtime(&now);       // Konversi ke struct tm (jam, menit, hari, ...)

    // Bandingkan dengan waktu terakhir yang ditampilkan
    if (t->tm_hour != lastDisplayedHour || t->tm_min != lastDisplayedMinute) {
        // Waktu berubah → update state dan beri tahu pemanggil
        lastDisplayedHour = t->tm_hour;
        lastDisplayedMinute = t->tm_min;
        return true;  // Perlu redraw jam
    }
    return false;  // Tidak ada perubahan
}

void drawClock()
{
    // Ambil waktu sistem saat ini
    time_t now = time(NULL);
    tm *t = localtime(&now);

    char timeBuffer[10];   // Buffer untuk "HH:MM\0" (max 6 char + null)
    char dateBuffer[20];   // Buffer untuk "DD/MM/YYYY\0" (max 11 char + null)

    // Format jam: %02d = integer 2 digit dengan leading zero
    // Contoh: jam 9 menit 5 → "09:05" (bukan "9:5")
    sprintf(timeBuffer, "%02d:%02d", t->tm_hour, t->tm_min);

    // Format tanggal dengan offset koreksi
    // tm_mon + 1  : karena tm_mon 0-based (0=Januari, 11=Desember)
    // tm_year + 1900 : karena tm_year adalah tahun sejak 1900
    sprintf(dateBuffer, "%02d/%02d/%04d",
            t->tm_mday, t->tm_mon + 1, t->tm_year + 1900);

    // --- Gambar teks jam & tanggal ---
    setcolor(WHITE);          // Warna teks: putih

    // Simpan background color lama, lalu set ke DARKGRAY
    // (samakan dengan warna taskbar agar teks tidak punya kotak background)
    int oldBk = getbkcolor();
    setbkcolor(DARKGRAY);

    // Posisi horizontal: right-align
    // Margin kanan 15px, lalu kurangi lebar teks agar rata kanan
    int rightMargin = getmaxx() - 15;

    // Jam di baris atas (Y = getmaxy() - 38, yaitu 12px dari bawah taskbar)
    outtextxy(rightMargin - textwidth(timeBuffer), getmaxy() - 38, timeBuffer);

    // Tanggal di baris bawah (Y = getmaxy() - 20, yaitu 30px dari bawah taskbar)
    outtextxy(rightMargin - textwidth(dateBuffer), getmaxy() - 20, dateBuffer);

    // Kembalikan background color ke nilai semula (defensive programming)
    // Mencegah efek samping ke modul lain yang menggambar teks setelah ini
    setbkcolor(oldBk);
}