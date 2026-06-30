/*
 * ============================================================
 * utils.cpp — Implementasi State Global & Utilitas
 * ============================================================
 * File ini mendefinisikan (bukan hanya mendeklarasikan) variabel
 * state global yang digunakan oleh berbagai modul, serta fungsi
 * pembantu untuk mendeteksi perubahan state.
 *
 * Variabel Global (definisi — pasangan extern di utils.h):
 *  - startMenuOpen : status Start Menu (true = sedang terbuka)
 *  - needsRedraw   : flag redraw (mungkin tidak terpakai — dead code?)
 *  - isRunning     : flag game loop (false = keluar program)
 *
 * Fungsi Deteksi Perubahan:
 *  - hasTimeChanged()      : cek jam/menit (DUPLIKAT dengan isTimeChanged di clock.cpp!)
 *  - hasMenuStateChanged() : cek status menu berubah
 *  - updateLastState()     : simpan state untuk perbandingan berikutnya
 *
 * CATATAN: Ada duplikasi antara hasTimeChanged() di sini dan
 * isTimeChanged() di clock.cpp. main.cpp menggunakan isTimeChanged()
 * dari clock.h, sehingga hasTimeChanged() di sini mungkin dead code.
 * ============================================================
 */

#include "../include/utils.h"
#include <time.h>  // time(), localtime()

// ==========================================
// DEFINISI Variabel Global
// ==========================================
// Ini adalah ALOKASI MEMORI sesungguhnya untuk variabel
// yang di-deklarasikan sebagai extern di utils.h.
// Tanpa definisi ini, linker akan error: "undefined reference".

bool startMenuOpen = false;  // Awal: menu tertutup
bool needsRedraw = true;     // Awal: perlu redraw (untuk initial draw)
bool isRunning = true;       // Awal: program berjalan

// ==========================================
// State untuk Deteksi Perubahan
// ==========================================
// static = hanya bisa diakses dari file ini (internal linkage)
// Nilai awal -1 memastikan deteksi perubahan pada pengecekan pertama

static int lastHour = -1;           // Jam terakhir yang dicek
static int lastMinute = -1;         // Menit terakhir yang dicek
static bool lastMenuState = false;  // Status menu terakhir
static float lastAnimProgress = 0.0f;  // Progress animasi terakhir

bool hasTimeChanged()
{
    // Ambil waktu sistem (sama persis dengan isTimeChanged() di clock.cpp)
    time_t now = time(NULL);
    tm *t = localtime(&now);

    // Bandingkan jam/menit dengan state terakhir
    if (t->tm_hour != lastHour || t->tm_min != lastMinute) {
        lastHour = t->tm_hour;
        lastMinute = t->tm_min;
        return true;  // Waktu berubah
    }
    return false;  // Waktu sama
}

bool hasMenuStateChanged()
{
    // Bandingkan status menu saat ini dengan status terakhir
    return startMenuOpen != lastMenuState;
}

void updateLastState()
{
    // Simpan status menu saat ini sebagai "terakhir"
    // Dipanggil setelah perubahan state diproses
    lastMenuState = startMenuOpen;
}