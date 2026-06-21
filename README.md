# Grafkom Startmenu

Proyek ini adalah simulasi bentuk antarmuka (Desktop dan Start Menu) yang dibuat menggunakan bahasa C++ dan library **WinBGIm** (`graphics.h`).

## 🛠️ Prasyarat (Prerequisites)

Sebelum menjalankan proyek ini, pastikan sistem (Windows) Anda sudah memiliki:
1. **MinGW** (Compiler C/C++) yang sudah ditambahkan ke *Environment Variables* (`PATH`).
2. Library **WinBGIm** (`graphics.h`, `winbgim.h`, dan `libbgi.a` yang sudah dikonfigurasi pada instalasi MinGW Anda).
3. **Visual Studio Code** (VS Code) dengan ekstensi **C/C++** dari Microsoft.

## 🚀 Cara Menjalankan (How to Run)

Anda bisa mengompilasi dan menjalankan program ini dengan dua cara:

### Cara 1: Melalui VS Code Build Task (Direkomendasikan)
Proyek ini sudah dilengkapi dengan konfigurasi `tasks.json` sehingga Anda dapat melakukan kompilasi otomatis.

1. Buka folder proyek ini di **Visual Studio Code**.
2. Tekan `Ctrl + Shift + B` untuk menjalankan proses *Build*.
3. (Opsional) Jika muncul pilihan, pilih task **"C/C++: g++.exe build active file"**.
4. Tunggu beberapa saat. Jika terminal tidak menunjukkan error, maka proses *compile* berhasil dan file `app.exe` akan muncul di dalam folder `bin/`.
5. Untuk menjalankan programnya, buka Terminal di VS Code (`Ctrl + ` `) dan jalankan:
   ```powershell
   .\bin\app.exe
   ```

### Cara 2: Melalui Terminal (Manual)
Buka terminal (PowerShell / Command Prompt) di lokasi root folder proyek ini, kemudian jalankan perintah kompilasi berikut:

```powershell
g++ -fdiagnostics-color=always -g src\*.cpp -I include -o bin\app.exe -lbgi -lgdi32 -lcomdlg32 -luuid -loleaut32 -lole32
```

Setelah itu, jalankan program menggunakan perintah:
```powershell
.\bin\app.exe
```

## 📁 Struktur Proyek
- `src/` : Berisi file sumber C++ (`.cpp`) seperti `main.cpp`, `desktop.cpp`, dll.
- `include/` : Berisi file header (`.h`).
- `assets/` : Menyimpan aset pendukung berupa `fonts/` dan `icons/`.
- `bin/` : Direktori output tempat hasil kompilasi (`app.exe`) diletakkan.
- `lib/` : Direktori khusus untuk tambahan file library luar jika ada.

## 📝 Catatan Penting
- Karena proyek ini menggunakan *BGI Graphics* (untuk sistem 32-bit/WinAPI sederhana), pastikan Anda melakukan kompilasi dengan versi GCC MinGW yang sesuai.
- Letak direktori ketika Anda me-*run* program (Working Directory) harus sama dengan lokasi folder proyek supaya C++ bisa mendeteksi lokasi folder `assets/` dengan benar.