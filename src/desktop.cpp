/*
 * ============================================================
 * desktop.cpp — Implementasi modul Desktop
 * ============================================================
 * Menggambar layer paling bawah dari tampilan:
 *  1. Background biru sebagai fallback (jika wallpaper gagal)
 *  2. Wallpaper dari assets/wallpaper.bmp (ditimpa di atas background)
 *  3. Lima ikon shortcut desktop beserta label teks
 *
 * Fungsi drawDesktop() hanya dipanggil SEKALI saat startup oleh main.cpp.
 * Hasilnya disalin ke buffer memori (bgImage) dan tinggal di-putimage()
 * setiap kali redraw — ini jauh lebih cepat daripada membaca file .bmp
 * berulang kali.
 *
 * Pola fallback: jika wallpaper.bmp tidak ditemukan, readimagefile()
 * tidak menggambar apa-apa, dan background biru tetap terlihat.
 * Ini adalah defensive programming — mencegah layar hitam kosong.
 * ============================================================
 */

#include <graphics.h>
#include "../include/desktop.h"

void drawDesktop()
{
    // --- Layer 1: Background fallback (biru) ---
    // Digambar dulu sebagai "jaring pengaman". Jika wallpaper gagal
    // dimuat (file tidak ada / rusak), background ini tetap terlihat.
    setfillstyle(SOLID_FILL, BLUE);        // Pola: isian solid, warna: biru
    bar(0, 0, getmaxx(), getmaxy());       // Kotak memenuhi seluruh layar

    // --- Layer 2: Wallpaper (ditimpa di atas background) ---
    // readimagefile() membaca file .bmp dan langsung menggambarnya.
    // Jika file tidak ditemukan → tidak melakukan apa-apa (tidak error).
    // Scaling otomatis: gambar asli diskalakan ke area (0,0)-(maxX,maxY).
    readimagefile("assets/wallpaper.bmp",
                  0, 0,                    // Koordinat kiri-atas
                  getmaxx(), getmaxy());   // Koordinat kanan-bawah

    // --- Layer 3: Ikon shortcut desktop ---
    // Setiap ikon terdiri dari:
    //  1. readimagefile() — gambar bitmap 60x60 piksel
    //  2. outtextxy()     — teks label di bawah gambar
    // Cast (char*) diperlukan karena outtextxy() mengharapkan char* non-const
    // (warisan API C lama), sedangkan string literal C++ bertipe const char*.
    setcolor(WHITE);  // Warna teks label ikon

    // Ikon 1: Browser (Y = 20)
    readimagefile("assets/browser.bmp",
                  20, 20,   // kiri-atas
                  80, 80);  // kanan-bawah (ukuran 60x60)
    outtextxy(25, 90, (char*)"Browser");

    // Ikon 2: Folder (Y = 130)
    readimagefile("assets/folder.bmp",
                  20, 130,
                  80, 190);
    outtextxy(30, 200, (char*)"Folder");

    // Ikon 3: Settings (Y = 240)
    readimagefile("assets/settings.bmp",
                  20, 240,
                  80, 300);
    outtextxy(25, 310, (char*)"Settings");

    // Ikon 4: Profile (Y = 350)
    readimagefile("assets/profile.bmp",
                  20, 350,
                  80, 410);
    outtextxy(30, 420, (char*)"Profile");

    // Ikon 5: Power (Y = 460)
    readimagefile("assets/power.bmp",
                  20, 460,
                  80, 520);
    outtextxy(35, 530, (char*)"Power");

    // CATATAN: Pola di atas berulang 5 kali dengan hanya koordinat Y
    // dan label yang berbeda. Idealnya gunakan array struct + loop for
    // untuk mengurangi duplikasi kode.
}