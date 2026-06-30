/*
 * ============================================================
 * utils.h — Deklarasi State Global & Fungsi Utilitas
 * ============================================================
 * File ini menyimpan variabel state yang di-share antar modul.
 *
 * Variabel extern (didefinisikan di utils.cpp):
 *  - startMenuOpen : status Start Menu (true = terbuka)
 *  - needsRedraw   : flag redraw (mungkin tidak terpakai di versi ini)
 *  - isRunning     : flag game loop (false = program akan keluar)
 *
 * Fungsi deteksi perubahan state (digunakan main loop):
 *  - hasTimeChanged()      : cek apakah jam/menit berubah
 *  - hasMenuStateChanged() : cek apakah status menu berubah
 *  - updateLastState()     : simpan state saat ini sebagai state terakhir
 *
 * CATATAN: hasTimeChanged() di sini mirip dengan isTimeChanged()
 * di clock.cpp — ini adalah duplikasi yang perlu dibersihkan.
 * ============================================================
 */

#ifndef UTILS_H
#define UTILS_H

// --- Variabel Global (definisi di utils.cpp) ---

// Status Start Menu: true = sedang terbuka, false = tertutup
extern bool startMenuOpen;

// Flag redraw: true = main loop harus menggambar ulang (mungkin tidak terpakai)
extern bool needsRedraw;

// Flag game loop: false = program keluar dari while loop dan berhenti
extern bool isRunning;

// --- Fungsi Deteksi Perubahan State ---

// Cek apakah waktu (jam/menit) berubah sejak pengecekan terakhir
bool hasTimeChanged();

// Cek apakah status Start Menu berubah (buka ↔ tutup)
bool hasMenuStateChanged();

// Simpan state saat ini sebagai state terakhir (untuk perbandingan frame berikutnya)
void updateLastState();

#endif