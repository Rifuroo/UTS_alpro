/*
 * =============================================================
 *  PROGRAM TERINTEGRASI - SISTEM PENDAFTARAN SEMINAR NASIONAL
 *  Studi Kasus 07 | SSI1144 Algoritma dan Pengantar Pemrograman
 *  Program Studi Sistem Informasi - Universitas Ma Chung
 * =============================================================
 *  Kolaborasi Kelompok:
 *    - Anggota 1 : Riflo Ahmad Nendriansyah (NIM: 322610018) -> Modul A
 *    - Anggota 2 : Praysilia Sifra Paradisa (NIM: 322610010) -> Modul B
 * =============================================================
 *  Deskripsi Integrasi:
 *    Menggabungkan Modul A (Proses Utama Pendaftaran Peserta,
 *    Validasi Input, Kalkulasi Diskon Bertingkat, dan Rincian Tagihan)
 *    dengan Modul B (Akumulasi Rekapitulasi Data, Penentuan Kategori
 *    Terbanyak dengan Tie-Breaker, Analisis Finansial, dan Metrik).
 * =============================================================
 */

#include <iostream>
#include <string>
#include <iomanip>
#include <cmath>
#include <sstream>

using namespace std;

// ---- Konstanta Tarif Dasar ----
const double TARIF_MAHASISWA      = 75000.0;
const double TARIF_DOSEN          = 125000.0;
const double TARIF_UMUM           = 175000.0;

// ---- Konstanta Diskon ----
const double DISKON_EARLY_BIRD    = 0.20;
const double DISKON_KELOMPOK      = 0.10;
const int    MIN_ANGGOTA_KELOMPOK = 5;

// ---- Konstanta Biaya Tambahan (Tanpa Diskon) ----
const double BIAYA_SERTIFIKAT     = 20000.0;
const double BIAYA_MAKAN          = 35000.0;

// ================================================================
//  Fungsi Bantu: Baca Satu Baris & Trim Spasi
// ================================================================
string bacaBaris(const string &pertanyaan) {
    string baris;
    cout << pertanyaan;
    getline(cin, baris);
    while (!baris.empty() && (baris.back() == ' ' || baris.back() == '\r')) {
        baris.pop_back();
    }
    size_t mulai = baris.find_first_not_of(' ');
    if (mulai == string::npos) return "";
    return baris.substr(mulai);
}

// ================================================================
//  Fungsi Bantu: Baca Nama (Tidak Boleh Kosong)
// ================================================================
string bacaNama(const string &pertanyaan) {
    string nama;
    while (true) {
        nama = bacaBaris(pertanyaan);
        if (!nama.empty()) return nama;
        cout << "  [!] Nama tidak boleh kosong. Silakan coba lagi.\n";
    }
}

// ================================================================
//  Fungsi Bantu: Konversi String ke Integer
// ================================================================
bool konversiInt(const string &teks, int &hasil) {
    if (teks.empty()) return false;
    istringstream ss(teks);
    int nilai;
    char sisa;
    if (ss >> nilai && !(ss >> sisa)) {
        hasil = nilai;
        return true;
    }
    return false;
}

// ================================================================
//  Fungsi Bantu: Baca Karakter Y/T (Case Insensitive)
// ================================================================
char bacaYaaTidak(const string &pertanyaan) {
    while (true) {
        string baris = bacaBaris(pertanyaan);
        if (baris.length() == 1) {
            char c = toupper(baris[0]);
            if (c == 'Y' || c == 'T') return c;
        }
        cout << "  [!] Input tidak valid. Harap ketik Y atau T.\n";
    }
}

// ================================================================
//  Fungsi Bantu: Baca Integer Positif (>0)
// ================================================================
int bacaIntPositif(const string &pertanyaan) {
    while (true) {
        string baris = bacaBaris(pertanyaan);
        int nilai;
        if (konversiInt(baris, nilai) && nilai > 0) {
            return nilai;
        }
        cout << "  [!] Input tidak valid. Masukkan bilangan bulat positif (>0).\n";
    }
}

// ================================================================
//  Fungsi Bantu: Baca Kategori (1, 2, atau 3)
// ================================================================
int bacaKategori() {
    while (true) {
        string baris = bacaBaris("  Kategori (1=Mahasiswa, 2=Dosen, 3=Umum) : ");
        int nilai;
        if (konversiInt(baris, nilai) && (nilai == 1 || nilai == 2 || nilai == 3)) {
            return nilai;
        }
        cout << "  [!] Pilihan kategori tidak valid. Pilih 1, 2, atau 3.\n";
    }
}

// ================================================================
//  Fungsi Bantu: Nama Kategori Teks
// ================================================================
string namaKategori(int kategori) {
    if (kategori == 1) return "Mahasiswa";
    if (kategori == 2) return "Dosen";
    return "Umum";
}

// ================================================================
//  Fungsi Bantu: Tarif Dasar Kategori
// ================================================================
double tarifDasar(int kategori) {
    if (kategori == 1) return TARIF_MAHASISWA;
    if (kategori == 2) return TARIF_DOSEN;
    return TARIF_UMUM;
}

// ================================================================
//  Fungsi Bantu: Cetak Garis Pemisah
// ================================================================
void cetakGaris(char karakter, int panjang) {
    for (int i = 0; i < panjang; i++) cout << karakter;
    cout << "\n";
}

// ================================================================
//  Fungsi Bantu: Format Rupiah (Contoh: 109000 -> "Rp109.000")
// ================================================================
string formatRupiah(double nilai) {
    long long bulat = (long long)round(nilai);
    string hasil = to_string(bulat);
    int posisi = (int)hasil.length() - 3;
    while (posisi > 0) {
        hasil.insert(posisi, ".");
        posisi -= 3;
    }
    return "Rp" + hasil;
}

// ================================================================
//  Fungsi Bantu: Format Persentase
// ================================================================
string formatPersen(double nilai) {
    ostringstream oss;
    oss << fixed << setprecision(1) << nilai << "%";
    return oss.str();
}

// ================================================================
//  Fungsi Modul B: Evaluasi Kategori Terbanyak (Termasuk Seri/Tie)
// ================================================================
string evaluasiKategoriTerbanyak(int mhs, int dsn, int umum) {
    int maxVal = mhs;
    if (dsn > maxVal) maxVal = dsn;
    if (umum > maxVal) maxVal = umum;

    if (maxVal == 0) return "Belum ada peserta";

    // Evaluasi seluruh kemungkinan seri
    if (mhs == dsn && dsn == umum) {
        return "Ketiga kategori imbang/seri (masing-masing " + to_string(mhs) + " orang)";
    }
    if (mhs == dsn && mhs == maxVal) {
        return "Mahasiswa & Dosen imbang terbanyak (masing-masing " + to_string(mhs) + " orang)";
    }
    if (mhs == umum && mhs == maxVal) {
        return "Mahasiswa & Umum imbang terbanyak (masing-masing " + to_string(mhs) + " orang)";
    }
    if (dsn == umum && dsn == maxVal) {
        return "Dosen & Umum imbang terbanyak (masing-masing " + to_string(dsn) + " orang)";
    }

    // Pemenang tunggal
    if (mhs == maxVal) return "Mahasiswa (" + to_string(mhs) + " orang)";
    if (dsn == maxVal) return "Dosen (" + to_string(dsn) + " orang)";
    return "Umum (" + to_string(umum) + " orang)";
}

// ================================================================
//  PROGRAM UTAMA (INTEGRASI MODUL A & MODUL B)
// ================================================================
int main() {
    // ---- Variabel Akumulator Modul B ----
    int    totalPesertaMahasiswa  = 0;
    int    totalPesertaDosen      = 0;
    int    totalPesertaUmum       = 0;
    int    totalEarlyBird         = 0;
    int    totalKelompokDiskon    = 0;
    int    totalSertifikat        = 0;
    int    totalMakan             = 0;

    double totalBiayaSertifikat   = 0.0;
    double totalBiayaMakan        = 0.0;
    double totalBruto             = 0.0;
    double totalDiskonEB          = 0.0;
    double totalDiskonKelompok    = 0.0;
    double totalDiskon            = 0.0;
    double totalPendapatan        = 0.0;

    double tagihanTerbesar        = 0.0;
    string namaPesertaTerbesar    = "-";
    int    nomorUrutTerbesar      = 0;

    int    nomorUrut              = 1;
    char   lanjut                 = 'Y';

    // ---- Banner Selamat Datang Program Terintegrasi ----
    cout << "\n";
    cetakGaris('=', 64);
    cout << "     SISTEM PENDAFTARAN & REKAPITULASI SEMINAR NASIONAL\n";
    cout << "          PROGRAM TERINTEGRASI (MODUL A + MODUL B)\n";
    cout << "      Studi Kasus 07 | SSI1144 | Universitas Ma Chung\n";
    cetakGaris('=', 64);

    // ================================================================
    //  PERULANGAN UTAMA: PROSES DATA PESERTA (MODUL A & MODUL B)
    // ================================================================
    while (lanjut == 'Y') {
        cout << "\n";
        cetakGaris('-', 64);
        cout << "  PENDAFTARAN TRANSAKSI #" << nomorUrut << "\n";
        cetakGaris('-', 64);

        // 1. INPUT DATA DENGAN VALIDASI KETAT (MODUL A)
        string namaPeserta   = bacaNama("  Nama peserta            : ");
        int    kategori      = bacaKategori();
        char   statusEB      = bacaYaaTidak("  Early bird? (Y/T)       : ");
        bool   earlyBird     = (statusEB == 'Y');
        int    jumlahAnggota = bacaIntPositif("  Jumlah anggota kelompok : ");
        bool   kelompok      = (jumlahAnggota >= MIN_ANGGOTA_KELOMPOK);
        char   statusSertif  = bacaYaaTidak("  Sertifikat cetak? (Y/T) : ");
        bool   pesanSertif   = (statusSertif == 'Y');
        char   statusMakan   = bacaYaaTidak("  Paket makan? (Y/T)      : ");
        bool   pesanMakan    = (statusMakan == 'Y');

        // 2. LOGIKA KALKULASI TAGIHAN INDIVIDUAL (MODUL A)
        double tarif = tarifDasar(kategori);

        // Diskon early bird: 20% dari tarif dasar
        double nominalDiskonEB = 0.0;
        if (earlyBird) {
            nominalDiskonEB = tarif * DISKON_EARLY_BIRD;
        }
        double tarifSetelahEB = tarif - nominalDiskonEB;

        // Diskon kelompok: 10% dari tarif setelah early bird (syarat >= 5 orang)
        double nominalDiskonKelompok = 0.0;
        if (kelompok) {
            nominalDiskonKelompok = tarifSetelahEB * DISKON_KELOMPOK;
        }
        double tarifBersih = tarifSetelahEB - nominalDiskonKelompok;

        // Biaya tambahan tanpa diskon
        double biayaTambahan = 0.0;
        if (pesanSertif) biayaTambahan += BIAYA_SERTIFIKAT;
        if (pesanMakan)  biayaTambahan += BIAYA_MAKAN;

        double totalDiskonPeserta = nominalDiskonEB + nominalDiskonKelompok;
        double totalTagihan = tarifBersih + biayaTambahan;

        // 3. AKUMULASI STATISTIK DAN KEUANGAN (MODUL B)
        if      (kategori == 1) totalPesertaMahasiswa++;
        else if (kategori == 2) totalPesertaDosen++;
        else                    totalPesertaUmum++;

        if (earlyBird) totalEarlyBird++;
        if (kelompok)  totalKelompokDiskon++;
        if (pesanSertif) {
            totalSertifikat++;
            totalBiayaSertifikat += BIAYA_SERTIFIKAT;
        }
        if (pesanMakan) {
            totalMakan++;
            totalBiayaMakan += BIAYA_MAKAN;
        }

        totalBruto          += (tarif + biayaTambahan);
        totalDiskonEB       += nominalDiskonEB;
        totalDiskonKelompok += nominalDiskonKelompok;
        totalDiskon         += totalDiskonPeserta;
        totalPendapatan     += totalTagihan;

        // Pelacak Transaksi Terbesar
        if (totalTagihan > tagihanTerbesar) {
            tagihanTerbesar     = totalTagihan;
            namaPesertaTerbesar = namaPeserta;
            nomorUrutTerbesar   = nomorUrut;
        }

        // 4. OUTPUT RINCIAN TAGIHAN INDIVIDUAL (OUTPUT MODUL A)
        cout << "\n";
        cetakGaris('=', 64);
        cout << "  RINCIAN TAGIHAN PESERTA - TRANSAKSI #" << nomorUrut << "\n";
        cetakGaris('=', 64);
        cout << left;
        cout << "  Nama peserta            : " << namaPeserta << "\n";
        cout << "  Kategori                : " << namaKategori(kategori) << "\n";
        cout << "  Jumlah anggota kelompok : " << jumlahAnggota << " orang"
             << (kelompok ? " (diskon kelompok 10% aktif)" : " (tidak memenuhi diskon kelompok)") << "\n";
        cout << "  Status early bird       : " << (earlyBird ? "Ya (diskon 20% aktif)" : "Tidak") << "\n";
        cetakGaris('-', 64);
        cout << right;
        cout << "  Tarif dasar             : " << setw(14) << formatRupiah(tarif) << "\n";

        if (earlyBird) {
            cout << "  Diskon early bird (20%) : " << setw(14) << ("-" + formatRupiah(nominalDiskonEB)) << "\n";
            cout << "  Tarif setelah EB        : " << setw(14) << formatRupiah(tarifSetelahEB) << "\n";
        }

        if (kelompok) {
            cout << "  Diskon kelompok (10%)   : " << setw(14) << ("-" + formatRupiah(nominalDiskonKelompok)) << "\n";
        }

        cout << "  Tarif bersih pendaftaran: " << setw(14) << formatRupiah(tarifBersih) << "\n";

        if (pesanSertif || pesanMakan) {
            cetakGaris('-', 64);
            cout << left << "  Biaya Layanan Tambahan (Tanpa Diskon):\n" << right;
            if (pesanSertif) {
                cout << "    Sertifikat cetak fisik: " << setw(14) << formatRupiah(BIAYA_SERTIFIKAT) << "\n";
            }
            if (pesanMakan) {
                cout << "    Paket makan/konsumsi  : " << setw(14) << formatRupiah(BIAYA_MAKAN) << "\n";
            }
        }

        cetakGaris('-', 64);
        cout << "  TOTAL DISKON DIBERIKAN  : " << setw(14) << ("-" + formatRupiah(totalDiskonPeserta)) << "\n";
        cout << "  TOTAL TAGIHAN FINAL     : " << setw(14) << formatRupiah(totalTagihan) << "\n";
        cetakGaris('=', 64);

        // 5. TANYA LANJUT TRANSAKSI
        nomorUrut++;
        cout << "\n";
        lanjut = bacaYaaTidak("  Input transaksi pendaftaran berikutnya? (Y/T) : ");
    }

    // ================================================================
    //  LAPORAN REKAPITULASI AKHIR (OUTPUT MODUL B)
    // ================================================================
    int totalPeserta = totalPesertaMahasiswa + totalPesertaDosen + totalPesertaUmum;

    cout << "\n\n";
    cetakGaris('=', 64);
    cout << "     LAPORAN REKAPITULASI & STATISTIK SEMINAR NASIONAL\n";
    cout << "                   OUTPUT AKHIR MODUL B\n";
    cetakGaris('=', 64);

    if (totalPeserta == 0) {
        cout << "\n  [i] Tidak ada transaksi pendaftaran yang diproses.\n\n";
        cetakGaris('=', 64);
        return 0;
    }

    // Perhitungan Statistik
    double persenMhs  = ((double)totalPesertaMahasiswa / totalPeserta) * 100.0;
    double persenDsn  = ((double)totalPesertaDosen / totalPeserta) * 100.0;
    double persenUmum = ((double)totalPesertaUmum / totalPeserta) * 100.0;

    double rataRataPendapatan = totalPendapatan / totalPeserta;
    double rataRataDiskon     = totalDiskon / totalPeserta;

    string kategoriTerbanyak  = evaluasiKategoriTerbanyak(totalPesertaMahasiswa,
                                                         totalPesertaDosen,
                                                         totalPesertaUmum);

    // 1. REKAPITULASI PESERTA & KATEGORI
    cout << "\n  [1] DISTRIBUSI PESERTA MENURUT KATEGORI\n";
    cetakGaris('-', 64);
    cout << left;
    cout << "  " << setw(18) << "Kategori"
         << setw(16) << "Jumlah Peserta"
         << setw(14) << "Persentase"
         << "Tarif Satuan\n";
    cetakGaris('-', 64);
    cout << "  " << setw(18) << "1. Mahasiswa"
         << setw(16) << (to_string(totalPesertaMahasiswa) + " orang")
         << setw(14) << formatPersen(persenMhs)
         << formatRupiah(TARIF_MAHASISWA) << "\n";
    cout << "  " << setw(18) << "2. Dosen"
         << setw(16) << (to_string(totalPesertaDosen) + " orang")
         << setw(14) << formatPersen(persenDsn)
         << formatRupiah(TARIF_DOSEN) << "\n";
    cout << "  " << setw(18) << "3. Umum"
         << setw(16) << (to_string(totalPesertaUmum) + " orang")
         << setw(14) << formatPersen(persenUmum)
         << formatRupiah(TARIF_UMUM) << "\n";
    cetakGaris('-', 64);
    cout << "  " << setw(18) << "TOTAL PESERTA"
         << setw(16) << (to_string(totalPeserta) + " orang")
         << setw(14) << "100.0%" << "-\n";
    cetakGaris('-', 64);
    cout << "  >> Kategori Peserta Terbanyak : " << kategoriTerbanyak << "\n";

    // 2. REKAPITULASI LAYANAN & FASILITAS
    cout << "\n  [2] REKAPITULASI FASILITAS & BENEFIT\n";
    cetakGaris('-', 64);
    cout << "  - Peserta Early Bird (Diskon 20%)  : " << setw(5) << totalEarlyBird
         << " orang (" << formatPersen(((double)totalEarlyBird / totalPeserta) * 100.0) << ")\n";
    cout << "  - Rombongan Diskon Kelompok (>=5)  : " << setw(5) << totalKelompokDiskon
         << " transaksi\n";
    cout << "  - Pemesan Sertifikat Cetak fisik   : " << setw(5) << totalSertifikat
         << " orang (" << formatRupiah(totalBiayaSertifikat) << ")\n";
    cout << "  - Pemesan Paket Makan / Konsumsi   : " << setw(5) << totalMakan
         << " orang (" << formatRupiah(totalBiayaMakan) << ")\n";

    // 3. REKAPITULASI KEUANGAN & ARUS PENDAPATAN
    cout << "\n  [3] REKAPITULASI FINANSIAL & ARUS PENDAPATAN\n";
    cetakGaris('-', 64);
    cout << right;
    cout << "  Total Nilai Transaksi Kotor (Bruto) : " << setw(15) << formatRupiah(totalBruto) << "\n";
    cout << "  Total Potongan Diskon Early Bird    : " << setw(15) << ("-" + formatRupiah(totalDiskonEB)) << "\n";
    cout << "  Total Potongan Diskon Kelompok      : " << setw(15) << ("-" + formatRupiah(totalDiskonKelompok)) << "\n";
    cetakGaris('-', 64);
    cout << "  TOTAL AKUMULASI DISKON DIKELUARKAN  : " << setw(15) << ("-" + formatRupiah(totalDiskon)) << "\n";
    cout << "  TOTAL PENDAPATAN BERSIH KEGIATAN    : " << setw(15) << formatRupiah(totalPendapatan) << "\n";
    cetakGaris('=', 64);

    // 4. METRIK ANALISIS & TRANSAKSI PUNCAK
    cout << left;
    cout << "\n  [4] STATISTIK & METRIK ANALISIS\n";
    cetakGaris('-', 64);
    cout << "  - Rata-rata Pendapatan per Peserta  : " << formatRupiah(rataRataPendapatan) << " / orang\n";
    cout << "  - Rata-rata Diskon per Peserta      : " << formatRupiah(rataRataDiskon) << " / orang\n";
    cout << "  - Transaksi Terbesar (Puncak)       : " << formatRupiah(tagihanTerbesar) << "\n";
    cout << "    (Oleh Peserta #" << nomorUrutTerbesar << " - " << namaPesertaTerbesar << ")\n";
    cetakGaris('=', 64);

    cout << "\n  Program selesai dieksekusi dengan sukses. Terima kasih.\n\n";
    return 0;
}
