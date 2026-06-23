#ifndef UTILS_H
#define UTILS_H

extern bool startMenuOpen;
extern bool needsRedraw;

// Fungsi untuk deteksi perubahan state
bool hasTimeChanged();
bool hasMenuStateChanged();
void updateLastState();

#endif