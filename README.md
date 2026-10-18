# Sistem Pendaftaran Seminar Nasional

**SSI1144 · Algoritma dan Pengantar Pemrograman**
Studi Kasus 07 | Program Studi Sistem Informasi | Universitas Ma Chung

---

## Deskripsi

Program C++ untuk menghitung biaya pendaftaran seminar nasional dan merekap pendapatan kegiatan. Program berjalan via console/terminal.

## Anggota Kelompok

| Nama | NIM | Modul |
|------|-----|-------|
| Riflo Ahmad Nendriansyah | 322610018 | Modul A |
| Praysilia Sifra | 322610010 | Modul B |

## Struktur Repository

```
seminar-nasional-uts/
├── index.html              Beranda GitHub Pages
├── analisis.html           Analisis Masalah
├── modul-a.html            Modul A
├── modul-b.html            Modul B
├── integrasi.html          Program Terintegrasi
├── refleksi.html           Refleksi
├── pernyataan-ai.html      Pernyataan Penggunaan AI
├── assets/
│   ├── css/style.css
│   ├── flowchart/
│   │   ├── flowchart-modul-a.svg
│   │   └── flowchart-modul-b.svg
│   └── screenshot/
├── src/
│   ├── modul_a.cpp
│   ├── modul_b.cpp
│   └── seminar_final.cpp
└── docs/
    ├── pseudocode-modul-a.txt
    └── pseudocode-modul-b.txt
```

## Cara Menjalankan Program

### Modul A (Proses Utama)
```bash
# Compile Modul A
g++ -o modul_a src/modul_a.cpp -std=c++17

# Jalankan
./modul_a
```

### Modul B (Rekapitulasi)
```bash
# Compile Modul B
g++ -o modul_b src/modul_b.cpp -std=c++17

# Jalankan
./modul_b
```

## GitHub Pages

> URL: `https://<username>.github.io/<repo>/`

## Ketentuan Biaya

| Kategori | Tarif | Early Bird | Kelompok ≥5 |
|----------|-------|-----------|------------|
| Mahasiswa | Rp75.000 | -20% | -10% dari tarif setelah EB |
| Dosen | Rp125.000 | -20% | -10% dari tarif setelah EB |
| Umum | Rp175.000 | -20% | -10% dari tarif setelah EB |

Sertifikat cetak: Rp20.000 · Paket makan: Rp35.000 (tidak terkena diskon)
