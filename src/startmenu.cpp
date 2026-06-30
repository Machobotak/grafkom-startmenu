/*
 * ============================================================
 * startmenu.cpp — Implementasi modul Start Menu
 * ============================================================
 * Modul paling kompleks dalam proyek (~220 baris). Menangani:
 *
 * ANIMASI:
 *  - Linear interpolation (LERP): progress 0.0 → 1.0
 *  - Kecepatan konstan: animSpeed = 0.1 per frame (~10 frame untuk penuh)
 *  - Animasi terpisah untuk menu utama dan pop-up "All Programs"
 *
 * LAYOUT DUA KOLOM (Windows XP style):
 *  - Header: kotak biru dengan teks "Administrator" dan avatar
 *  - Kolom kiri (putih, 220px): menu utama + "All Programs"
 *  - Kolom kanan (biru muda, 260px): menu sekunder
 *  - Footer: kotak biru dengan "Log Off" dan "Turn Off Computer"
 *
 * POP-UP "ALL PROGRAMS":
 *  - Muncul dari batas kolom kiri saat mouse hover di "All Programs"
 *  - Animasi lebih cepat (1.5x speed)
 *  - Shadow sederhana (kotak abu-abu offset 5px)
 *  - Border biru + background putih
 *  - 8 item menu tambahan
 *
 * DETEKSI HOVER:
 *  - drawMenuItem() mengecek apakah mouse dalam bounding box item
 *  - Hover → background berubah warna (highlight)
 *  - Mengembalikan bool: true jika mouse di atas item
 * ============================================================
 */

#include <graphics.h>
#include <math.h>    // (disediakan untuk easing functions, belum dipakai)
#include "../include/startmenu.h"
#include "../include/utils.h"  // startMenuOpen (status global menu)
#include <stdlib.h>            // exit() untuk "Turn Off Computer"

// ==========================================
// VARIABEL STATE ANIMASI & HOVER
// ==========================================
// static = internal linkage, hanya bisa diakses dari file ini
// Semua variabel ini bertahan sepanjang program berjalan

static float animProgress      = 0.0f;  // Progress animasi menu utama (0.0 - 1.0)
static float popupAnimProgress = 0.0f;  // Progress animasi pop-up All Programs
static bool  isHoveringPrograms = false; // Mouse sedang di atas "All Programs"?
static bool  lastHoveringPrograms = false; // State hover sebelumnya (deteksi perubahan)
static int   lastMouseX = -1;  // Posisi mouse X sebelumnya
static int   lastMouseY = -1;  // Posisi mouse Y sebelumnya

// ==========================================
// HELPER: drawMenuItem()
// ==========================================
// Menggambar satu item menu dengan deteksi hover.
// Return true jika mouse sedang di atas item (hovered).
//
// PARAMETER:
//   x,y,w,h    : Posisi & ukuran kotak item (bounding box)
//   text       : Teks yang ditampilkan
//   mx, my     : Posisi mouse saat ini
//   bgNormal, fgNormal : Warna background & teks saat NORMAL
//   bgHover, fgHover   : Warna background & teks saat DI-HOVER
//   textX, textY : Posisi teks (terpisah dari kotak, untuk padding)
//
// Mengapa textX/textY terpisah dari x,y?
// Kotak background perlu menutupi area hover penuh, sedangkan
// teks perlu sedikit padding dari tepi kiri untuk estetika.
static bool drawMenuItem(int x, int y, int w, int h, const char* text, int mx, int my, int bgNormal, int fgNormal, int bgHover, int fgHover, int textX, int textY) {
    // Bounding box hit test: apakah mouse di dalam kotak?
    bool hovered = (mx >= x && mx <= x + w && my >= y && my <= y + h);
    if (hovered) {
        // --- State HOVER: background terang, teks kontras ---
        setfillstyle(SOLID_FILL, bgHover);
        bar(x, y, x + w, y + h);
        setbkcolor(bgHover);  // Background teks = background kotak
        setcolor(fgHover);    // Warna teks
    } else {
        // --- State NORMAL: background default ---
        setfillstyle(SOLID_FILL, bgNormal);
        bar(x, y, x + w, y + h);
        setbkcolor(bgNormal);
        setcolor(fgNormal);
    }
    // Gambar teks di posisi yang ditentukan (mungkin berbeda dari x,y)
    // Cast (char*) diperlukan karena API lama mengharapkan char* non-const
    outtextxy(textX, textY, (char*)text);
    return hovered;  // Kembalikan status hover untuk aksi lanjutan
}

// ==========================================
// FUNGSI UTAMA: drawStartMenu()
// ==========================================
// Dipanggil SETIAP FRAME oleh main loop.
// Menangani: update animasi, layout menu, hover, pop-up.
void drawStartMenu()
{
    // ==========================================
    // FASE 1: Update Progress Animasi Menu Utama
    // ==========================================
    float animSpeed = 0.1f;  // Kecepatan: 0.1 per frame (~10 frame untuk animasi penuh)
    if (startMenuOpen) {
        // Menu dibuka → animProgress naik dari 0 ke 1
        if (animProgress < 1.0f) animProgress += animSpeed;
        if (animProgress > 1.0f) animProgress = 1.0f;  // Clamp: jaga di rentang [0,1]
    } else {
        // Menu ditutup → animProgress turun dari 1 ke 0
        if (animProgress > 0.0f) animProgress -= animSpeed;
        if (animProgress < 0.0f) animProgress = 0.0f;  // Clamp
        isHoveringPrograms = false;  // Reset hover state saat menu tertutup
    }

    // Early exit: jika menu sepenuhnya tersembunyi, tidak perlu menggambar apa pun
    if (animProgress <= 0.0f) return;

    // ==========================================
    // FASE 2: Hitung Dimensi Menu (dinamis)
    // ==========================================
    int mx = mousex();  // Posisi mouse X saat ini
    int my = mousey();  // Posisi mouse Y saat ini

    int startY = getmaxy() - 50;       // Y taskbar (bagian bawah layar)
    int targetHeight = 450;            // Tinggi menu saat terbuka penuh
    int currentHeight = (int)(targetHeight * animProgress);  // Tinggi sesuai progress

    // Menu "tumbuh" dari taskbar ke ATAS
    // Saat animProgress=0: menuY = startY (tersembunyi)
    // Saat animProgress=1: menuY = startY - 450 (terbuka penuh)
    int menuY = startY - currentHeight;
    int menuX = 0;
    int menuWidth = 480;  // Lebar menu (diperlebar dari desain awal)

    int leftColWidth = 220;                           // Lebar kolom kiri
    int rightColWidth = menuWidth - leftColWidth;     // Lebar kolom kanan = 260
    int headerHeight = 60;                            // Tinggi header biru
    int footerHeight = 45;                            // Tinggi footer biru

    // ==========================================
    // FASE 3: Gambar Header (Biru, gaya XP)
    // ==========================================
    setfillstyle(SOLID_FILL, BLUE);
    bar(menuX, menuY, menuX + menuWidth, menuY + headerHeight);

    int oldBk = getbkcolor();  // Simpan background color lama (defensive)

    // Header muncul setelah animasi > 20% (efek fade-in bertahap)
    if (animProgress > 0.2f) {
        setbkcolor(BLUE);  // Samakan background teks dengan header
        setcolor(WHITE);
        settextstyle(SANS_SERIF_FONT, HORIZ_DIR, 2);  // Font sans-serif, horizontal, ukuran 2
        outtextxy(menuX + 60, menuY + 20, (char*)"Administrator");

        // Avatar placeholder (kotak abu-abu, nanti bisa diganti foto)
        setfillstyle(SOLID_FILL, LIGHTGRAY);
        bar(menuX + 10, menuY + 10, menuX + 50, menuY + 50);
    }

    // ==========================================
    // FASE 4: Gambar Kolom Kiri (Putih)
    // ==========================================
    setfillstyle(SOLID_FILL, WHITE);
    bar(menuX, menuY + headerHeight,
        menuX + leftColWidth, menuY + currentHeight - footerHeight);

    // ==========================================
    // FASE 5: Gambar Kolom Kanan (Biru Muda)
    // ==========================================
    setfillstyle(SOLID_FILL, LIGHTBLUE);
    bar(menuX + leftColWidth, menuY + headerHeight,
        menuX + menuWidth, menuY + currentHeight - footerHeight);

    // ==========================================
    // FASE 6: Gambar Footer (Biru)
    // ==========================================
    setfillstyle(SOLID_FILL, BLUE);
    bar(menuX, menuY + currentHeight - footerHeight,
        menuX + menuWidth, menuY + currentHeight);

    // ==========================================
    // FASE 7: Siapkan Pop-up "All Programs"
    // ==========================================
    settextstyle(SANS_SERIF_FONT, HORIZ_DIR, 1);  // Font lebih kecil untuk isi menu
    int popupWidth = 180;
    int popupTargetHeight = 250;
    int popupX = menuX + leftColWidth;  // Muncul dari batas kanan kolom kiri
    int popupY = menuY + currentHeight - footerHeight - popupTargetHeight;

    // Deteksi apakah mouse di area pop-up (untuk menentukan arah hover)
    bool hoveringPopupArea = false;
    if (popupAnimProgress > 0.0f) {
        // Menggunakan targetHeight (bukan currentHeight) untuk konsistensi
        hoveringPopupArea = (mx >= popupX && mx <= popupX + popupWidth &&
                             my >= popupY && my <= popupY + popupTargetHeight);
    }

    // ==========================================
    // FASE 8: Gambar Item Menu (jika animasi > 80%)
    // ==========================================
    // Item muncul setelah animasi hampir selesai (efek staggered)
    bool hoveringButton = false;
    if (animProgress > 0.8f) {
        int footerY = menuY + currentHeight - footerHeight;

        // --- Tombol "Log Off" di kiri footer ---
        drawMenuItem(menuX + 30, footerY + 5, 80, 30, "Log Off",
                     mx, my, BLUE, WHITE, LIGHTBLUE, WHITE,
                     menuX + 40, menuY + currentHeight - 30);

        // --- Tombol "Turn Off Computer" di kanan footer ---
        // drawMenuItem() mengembalikan true jika mouse di atas tombol.
        // Jika YA dan mouse diklik → keluar program.
        if (drawMenuItem(
                menuX + 250,
                footerY + 5,
                150,
                30,
                "Turn Off Computer",
                mx,
                my,
                BLUE,
                WHITE,
                LIGHTBLUE,
                WHITE,
                menuX + 260,
                menuY + currentHeight - 30))
        {
            // Mouse di atas tombol "Turn Off Computer" + ada klik kiri
            if (ismouseclick(WM_LBUTTONDOWN))
            {
                clearmouseclick(WM_LBUTTONDOWN);  // Hapus event dari antrean
                closegraph();                      // Tutup jendela grafis
                exit(0);                           // Keluar program
            }
        }

        // --- Item Menu Kolom Kiri (5 item) ---
        // Pola: drawMenuItem(x, y, w, h, text, mx, my, bgNormal, fgNormal, bgHover, fgHover, textX, textY)
        int leftX = menuX + 5;
        int leftW = leftColWidth - 10;
        drawMenuItem(leftX, menuY + headerHeight + 15, leftW, 25, "Internet Explorer", mx, my, WHITE, BLACK, BLUE, WHITE, menuX + 15, menuY + headerHeight + 20);
        drawMenuItem(leftX, menuY + headerHeight + 45, leftW, 25, "Outlook Express", mx, my, WHITE, BLACK, BLUE, WHITE, menuX + 15, menuY + headerHeight + 50);
        drawMenuItem(leftX, menuY + headerHeight + 75, leftW, 25, "Windows Media Player", mx, my, WHITE, BLACK, BLUE, WHITE, menuX + 15, menuY + headerHeight + 80);
        drawMenuItem(leftX, menuY + headerHeight + 105, leftW, 25, "MSN Explorer", mx, my, WHITE, BLACK, BLUE, WHITE, menuX + 15, menuY + headerHeight + 110);
        drawMenuItem(leftX, menuY + headerHeight + 135, leftW, 25, "Windows Movie Maker", mx, my, WHITE, BLACK, BLUE, WHITE, menuX + 15, menuY + headerHeight + 140);

        // --- Garis Pembatas ---
        setcolor(LIGHTGRAY);
        line(menuX + 10, menuY + headerHeight + 170,
             menuX + leftColWidth - 10, menuY + headerHeight + 170);

        // --- Tombol "All Programs" (di bawah kolom kiri) ---
        int allProgY = menuY + currentHeight - footerHeight - 30;
        setbkcolor(WHITE);
        setcolor(BLACK);
        outtextxy(menuX + 15, allProgY, (char*)"All Programs     >");

        // Deteksi hover pada area "All Programs"
        hoveringButton = (mx >= menuX && mx <= menuX + leftColWidth &&
                          my >= allProgY - 15 && my <= allProgY + 15);

        // Jika mouse di area pop-up, sembunyikan hover di kolom kanan
        // (supaya tidak ada highlight ganda yang membingungkan)
        int rightMx = hoveringPopupArea ? -1 : mx;
        int rightMy = hoveringPopupArea ? -1 : my;

        // --- Item Menu Kolom Kanan (8 item) ---
        int rightX = menuX + leftColWidth + 5;
        int rightW = rightColWidth - 10;
        drawMenuItem(rightX, menuY + headerHeight + 15, rightW, 25, "My Documents", rightMx, rightMy, LIGHTBLUE, WHITE, BLUE, WHITE, menuX + leftColWidth + 15, menuY + headerHeight + 20);
        drawMenuItem(rightX, menuY + headerHeight + 45, rightW, 25, "My Recent Documents", rightMx, rightMy, LIGHTBLUE, WHITE, BLUE, WHITE, menuX + leftColWidth + 15, menuY + headerHeight + 50);
        drawMenuItem(rightX, menuY + headerHeight + 75, rightW, 25, "My Pictures", rightMx, rightMy, LIGHTBLUE, WHITE, BLUE, WHITE, menuX + leftColWidth + 15, menuY + headerHeight + 80);
        drawMenuItem(rightX, menuY + headerHeight + 105, rightW, 25, "My Music", rightMx, rightMy, LIGHTBLUE, WHITE, BLUE, WHITE, menuX + leftColWidth + 15, menuY + headerHeight + 110);
        drawMenuItem(rightX, menuY + headerHeight + 135, rightW, 25, "My Computer", rightMx, rightMy, LIGHTBLUE, WHITE, BLUE, WHITE, menuX + leftColWidth + 15, menuY + headerHeight + 140);
        drawMenuItem(rightX, menuY + headerHeight + 175, rightW, 25, "Control Panel", rightMx, rightMy, LIGHTBLUE, WHITE, BLUE, WHITE, menuX + leftColWidth + 15, menuY + headerHeight + 180);
        drawMenuItem(rightX, menuY + headerHeight + 205, rightW, 25, "Connect To", rightMx, rightMy, LIGHTBLUE, WHITE, BLUE, WHITE, menuX + leftColWidth + 15, menuY + headerHeight + 210);
        drawMenuItem(rightX, menuY + headerHeight + 235, rightW, 25, "Printers and Faxes", rightMx, rightMy, LIGHTBLUE, WHITE, BLUE, WHITE, menuX + leftColWidth + 15, menuY + headerHeight + 240);
    }

    // Kembalikan background color ke nilai semula
    // Penting: mencegah efek samping ke modul lain yang menggambar teks
    setbkcolor(oldBk);

    // ==========================================
    // FASE 9: Update State Hover "All Programs"
    // ==========================================
    if (animProgress > 0.8f) {
        isHoveringPrograms = hoveringButton || hoveringPopupArea;
    } else {
        isHoveringPrograms = false;
    }

    // ==========================================
    // FASE 10: Update Animasi Pop-up
    // ==========================================
    if (isHoveringPrograms) {
        // Pop-up terbuka: animasi naik (1.5x lebih cepat dari menu utama)
        if (popupAnimProgress < 1.0f) popupAnimProgress += animSpeed * 1.5f;
        if (popupAnimProgress > 1.0f) popupAnimProgress = 1.0f;
    } else {
        // Pop-up tertutup: animasi turun
        if (popupAnimProgress > 0.0f) popupAnimProgress -= animSpeed * 1.5f;
        if (popupAnimProgress < 0.0f) popupAnimProgress = 0.0f;
    }

    // ==========================================
    // FASE 11: Gambar Pop-up "All Programs"
    // ==========================================
    if (popupAnimProgress > 0.0f) {
        int popupHeight = (int)(popupTargetHeight * popupAnimProgress);

        // --- Shadow (kotak abu-abu offset 5px ke kanan-bawah) ---
        // Digambar PERTAMA agar ditimpa oleh background putih
        // Teknik shadow murah: cukup offset dan warna berbeda
        setfillstyle(SOLID_FILL, DARKGRAY);
        bar(popupX + 5, popupY + popupTargetHeight - popupHeight + 5,
            popupX + popupWidth + 5, popupY + popupTargetHeight + 5);

        // --- Background pop-up (putih) ---
        setfillstyle(SOLID_FILL, WHITE);
        bar(popupX, popupY + popupTargetHeight - popupHeight,
            popupX + popupWidth, popupY + popupTargetHeight);

        // --- Border (biru) ---
        setcolor(BLUE);
        rectangle(popupX, popupY + popupTargetHeight - popupHeight,
                  popupX + popupWidth, popupY + popupTargetHeight);

        // --- Isi pop-up (8 item menu) ---
        // Hanya digambar setelah animasi hampir selesai (> 80%)
        if (popupAnimProgress > 0.8f) {
            settextstyle(SANS_SERIF_FONT, HORIZ_DIR, 1);
            int popX = popupX + 5;
            int popW = popupWidth - 10;
            drawMenuItem(popX, popupY + 15, popW, 25, "Accessories", mx, my, WHITE, BLACK, BLUE, WHITE, popupX + 15, popupY + 20);
            drawMenuItem(popX, popupY + 45, popW, 25, "Games", mx, my, WHITE, BLACK, BLUE, WHITE, popupX + 15, popupY + 50);
            drawMenuItem(popX, popupY + 75, popW, 25, "Startup", mx, my, WHITE, BLACK, BLUE, WHITE, popupX + 15, popupY + 80);
            drawMenuItem(popX, popupY + 105, popW, 25, "Internet Explorer", mx, my, WHITE, BLACK, BLUE, WHITE, popupX + 15, popupY + 110);
            drawMenuItem(popX, popupY + 135, popW, 25, "MSN", mx, my, WHITE, BLACK, BLUE, WHITE, popupX + 15, popupY + 140);
            drawMenuItem(popX, popupY + 165, popW, 25, "Outlook Express", mx, my, WHITE, BLACK, BLUE, WHITE, popupX + 15, popupY + 170);
            drawMenuItem(popX, popupY + 195, popW, 25, "Remote Assistance", mx, my, WHITE, BLACK, BLUE, WHITE, popupX + 15, popupY + 200);
            drawMenuItem(popX, popupY + 225, popW, 25, "Windows Media Player", mx, my, WHITE, BLACK, BLUE, WHITE, popupX + 15, popupY + 230);
            setbkcolor(oldBk);  // Kembalikan background color
        }
    }

    // ==========================================
    // FASE 12: Update State Tracking
    // ==========================================
    // Simpan state saat ini sebagai "terakhir" untuk perbandingan
    // di frame berikutnya (digunakan oleh isHoveringProgramsChanged)
    lastHoveringPrograms = isHoveringPrograms;
    lastMouseX = mx;
    lastMouseY = my;
}

// ==========================================
// TOGGLE: buka/tutup Start Menu
// ==========================================
// Dipanggil saat user mengklik tombol Start atau menekan Windows key
void toggleStartMenu()
{
    startMenuOpen = !startMenuOpen;  // Flip state: true ↔ false
}

// ==========================================
// QUERY: progress animasi saat ini
// ==========================================
// Digunakan main loop untuk mendeteksi apakah animasi sedang berjalan.
// 0.0 = menu tersembunyi, 1.0 = menu terbuka penuh,
// nilai di antaranya = sedang animasi.
float getAnimProgress()
{
    return animProgress;
}

// ==========================================
// QUERY: deteksi perubahan hover
// ==========================================
// Digunakan main loop untuk menentukan apakah perlu redraw.
// Return true jika:
//  - Mouse bergerak (posisi berbeda dari frame sebelumnya)
//  - Hover state berubah (masuk/keluar area "All Programs")
//  - Pop-up sedang animasi (perlu redraw terus-menerus)
bool isHoveringProgramsChanged()
{
    int mx = mousex();
    int my = mousey();

    // Cek apakah mouse bergerak
    bool mouseMoved = (mx != lastMouseX || my != lastMouseY);

    // Cek apakah hover state berubah
    bool hoverChanged = (isHoveringPrograms != lastHoveringPrograms);

    // Return true jika ada perubahan ATAU pop-up sedang animasi
    return mouseMoved || hoverChanged || popupAnimProgress > 0.0f;
}