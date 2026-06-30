/*
 * ============================================================
 * desktop.h — Deklarasi modul Desktop
 * ============================================================
 * Modul ini menggambar layer paling bawah dari tampilan:
 *  - Background biru (fallback jika wallpaper gagal dimuat)
 *  - Wallpaper dari file assets/wallpaper.bmp
 *  - Lima ikon shortcut (Browser, Folder, Settings, Profile, Power)
 *
 * Hasil gambar desktop disimpan ke buffer memori oleh main.cpp
 * agar tidak perlu digambar ulang setiap frame (optimasi).
 * ============================================================
 */

#ifndef DESKTOP_H
#define DESKTOP_H

// Menggambar seluruh desktop: wallpaper + ikon shortcut
void drawDesktop();

#endif