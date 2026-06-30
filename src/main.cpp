/*
 * ============================================================
 * main.cpp — Entry Point & Game Loop
 * ============================================================
 * Pusat kendali seluruh program. File ini:
 *  1. Membuat jendela grafis 1280x720 dengan initwindow()
 *  2. Menggambar desktop sekali, lalu menyimpannya ke buffer memori
 *     (optimasi: hindari baca file .bmp setiap frame)
 *  3. Menjalankan game loop ~60 FPS dengan double buffering
 *     (menggambar di halaman tersembunyi, lalu menukarnya)
 *  4. Mendeteksi perubahan state (mouse, keyboard, waktu, animasi)
 *     dan hanya me-redraw jika diperlukan (dirty flag pattern)
 *
 * Double Buffering:
 *  - setactivepage(0/1) : pilih halaman untuk menggambar (tidak terlihat)
 *  - setvisualpage(0/1) : pilih halaman yang ditampilkan ke layar
 *  - Dengan dua halaman, user tidak pernah melihat proses menggambar
 *    (menghilangkan flicker)
 *
 * Game Loop (while isRunning):
 *  1. Baca input (mouse + keyboard)
 *  2. Cek perubahan waktu (jam)
 *  3. Cek status animasi (Start Menu)
 *  4. Tentukan perlu redraw atau tidak
 *  5. Jika perlu: restore desktop → gambar taskbar → gambar menu → tukar halaman
 *  6. delay(16) → target 60 FPS
 * ============================================================
 */

#include <iostream>
#include <graphics.h>
#include <stdlib.h> // untuk malloc, free

#include "../include/desktop.h"
#include "../include/taskbar.h"
#include "../include/startmenu.h"
#include "../include/mouse.h"
#include "../include/clock.h"
#include "../include/utils.h"

// Flag untuk track apa yang perlu di-redraw (dirty flag pattern)
// static = internal linkage, hanya bisa diakses dari file ini
static bool needsFullRedraw = true;   // Perlu redraw seluruh layar?
static bool needsMenuRedraw = false;  // Perlu redraw area Start Menu?
static bool needsClockRedraw = false; // Perlu redraw jam?

int main()
{
    std::cout << "Program mulai\n";

    // Buat jendela grafis 1280x720 (resolusi HD standar)
    initwindow(1280, 720, "Start Menu Clone");

    std::cout << "Window dibuat\n";

    // Fase 1: Gambar desktop SEKALI dan simpan ke buffer memori
    // Desktop jarang berubah (hanya wallpaper + ikon), jadi kita
    // simpan sebagai bitmap di RAM agar cepat di-restore setiap redraw
    cleardevice();
    drawDesktop();

    // Hitung ukuran buffer yang dibutuhkan untuk seluruh layar
    unsigned int bgSize = imagesize(0, 0, getmaxx(), getmaxy());
    void* bgImage = malloc(bgSize);  // Alokasi memori (gaya C)
    if (bgImage) {
        // Salin piksel layar ke buffer (snapshot desktop)
        getimage(0, 0, getmaxx(), getmaxy(), bgImage);
    }
    // Jika malloc gagal (bgImage == NULL), program tetap berjalan
    // dengan fallback: desktop akan di-gambar ulang manual setiap redraw

    int page = 0;  // Halaman buffer aktif (0 atau 1, di-toggle setiap frame)

    // Fase 2: Initial full redraw dengan double buffering
    // Pola: gambar di halaman tersembunyi → tukar ke halaman terlihat
    setactivepage(1 - page);  // Pilih halaman tersembunyi untuk menggambar
    if (bgImage) {
        // Restore desktop dari buffer (jauh lebih cepat dari drawDesktop)
        putimage(0, 0, bgImage, COPY_PUT);
    } else {
        // Fallback jika buffer tidak tersedia
        cleardevice();
        drawDesktop();
    }
    drawTaskbar();
    drawStartMenu();
    setvisualpage(1 - page);  // Tampilkan hasil gambar ke layar
    page = 1 - page;          // Toggle halaman untuk frame berikutnya

    // Fase 3: Game Loop utama (~60 FPS)
    while (isRunning)
    {
        // --- Deteksi perubahan state (dirty flag pattern) ---
        // Kita tidak selalu menggambar ulang — hanya jika ada perubahan

        // Cek input: apakah mouse diklik / keyboard ditekan?
        bool mouseAction = handleMouse();
        bool keyAction = handleKeyboard();

        // Cek waktu: apakah menit sudah berganti? (jam perlu update)
        bool timeChanged = isTimeChanged();

        // Cek animasi: apakah Start Menu sedang transisi?
        float currentAnimProgress = getAnimProgress();

        // Deteksi animasi yang sedang berjalan (progress di antara 0 dan 1)
        // static: nilai lastAnimProgress bertahan antar pemanggilan
        static float lastAnimProgress = 0.0f;
        bool isAnimating = (currentAnimProgress > 0.0f && currentAnimProgress < 1.0f) ||
                          (currentAnimProgress != lastAnimProgress);

        // Deteksi ketika animasi BARU SAJA selesai (transisi dari animating ke done)
        // Ini penting: tanpa deteksi ini, frame terakhir animasi tidak akan digambar
        // dan menu akan "menghilang tiba-tiba" alih-alih menutup mulus
        static bool wasAnimating = false;
        bool animationJustFinished = (wasAnimating && !isAnimating);
        wasAnimating = isAnimating;

        lastAnimProgress = currentAnimProgress;

        // Cek perubahan hover state pada pop-up "All Programs"
        bool hoverChanged = isHoveringProgramsChanged();

        // --- Tentukan flag redraw berdasarkan perubahan yang terdeteksi ---
        needsMenuRedraw = mouseAction || keyAction || isAnimating || hoverChanged || animationJustFinished;
        needsClockRedraw = timeChanged;

        // Dengan double buffering, jika ada bagian yang perlu di-redraw, kita redraw SEMUA
        // (karena kita menggambar di halaman tersembunyi yang perlu diisi ulang sepenuhnya)
        needsFullRedraw = needsMenuRedraw || needsClockRedraw;

        if (needsFullRedraw) {
            // --- Double buffer redraw ---
            setactivepage(1 - page);  // Gambar di halaman tersembunyi

            // Restore desktop dari buffer (layer 1: background)
            if (bgImage) {
                putimage(0, 0, bgImage, COPY_PUT);
            } else {
                cleardevice();
                drawDesktop();
            }

            // Gambar UI di atas desktop (layer 2 & 3)
            drawTaskbar();    // Layer 2: taskbar
            drawStartMenu();  // Layer 3: Start Menu (di atas taskbar)

            setvisualpage(1 - page);  // Tukar: tampilkan hasil ke layar
            page = 1 - page;          // Toggle halaman
        }

        // Tunda 16 milidetik → target ~60 frame per detik (1000ms / 60 ≈ 16.7)
        // CATATAN: ini asumsi operasi lain instan. Untuk akurasi lebih baik,
        // gunakan delta time berbasis clock().
        delay(16);
    }

    // Cleanup: bebaskan memori dan tutup jendela grafis
    if (bgImage) {
        free(bgImage);  // Pasangan malloc()
    }
    closegraph();       // Tutup jendela grafis
    return 0;
}