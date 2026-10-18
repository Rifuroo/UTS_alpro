/*
 * =============================================================
 *  MODUL B - Sistem Pendaftaran Seminar Nasional
 *  Modul Rekapitulasi & Analisis Statistik Kegiatan
 *  Studi Kasus 07 | SSI1144 Algoritma dan Pengantar Pemrograman
 *  Program Studi Sistem Informasi - Universitas Ma Chung
 * =============================================================
 *  Penanggung Jawab : Praysilia Sifra
 *  Peran            : Modul B (Proses Rekapitulasi & Statistik)
 * =============================================================
 *
 *  Ketentuan Modul B (Sesuai Spesifikasi Kasus 07):
 *    1. Menghitung & menampilkan jumlah peserta mahasiswa, dosen, dan umum.
 *    2. Menghitung & menampilkan jumlah peserta early bird.
 *    3. Menghitung & menampilkan jumlah pemesan sertifikat dan paket makan.
 *    4. Menghitung & menampilkan total diskon yang diberikan.
 *    5. Menghitung & menampilkan total pendapatan kegiatan.
 *    6. Menentukan & menampilkan kategori peserta terbanyak
 *       (mencakup evaluasi kondisi seimbang / tie-breaker).
 *    7. Menyajikan metrik pelengkap: persentase distribusi peserta,
 *       rata-rata pendapatan per peserta, dan transaksi terbesar.
 *
 *  Validasi Input:
 *    - Nama peserta tidak boleh kosong
 *    - Kategori harus 1 (Mahasiswa), 2 (Dosen), atau 3 (Umum)
 *    - Status early bird : Y atau T
 *    - Jumlah anggota kelompok : minimal 1
 *    - Pilihan sertifikat dan makan : Y atau T
 *    - Pilihan lanjut : Y atau T
 * =============================================================
 */

#include <iostream>
#include <string>
#include <iomanip>
#include <cmath>
#include <sstream>

using namespace std;

// ---- Konstanta tarif dasar ----
const double TARIF_MAHASISWA      = 75000.0;
const double TARIF_DOSEN          = 125000.0;
const double TARIF_UMUM           = 175000.0;

// ---- Konstanta diskon ----
const double DISKON_EARLY_BIRD    = 0.20;
const double DISKON_KELOMPOK      = 0.10;
const int    MIN_ANGGOTA_KELOMPOK = 5;

// ---- Konstanta biaya tambahan ----
const double BIAYA_SERTIFIKAT     = 20000.0;
const double BIAYA_MAKAN          = 35000.0;

// ================================================================
//  Fungsi bantu: membaca satu baris & trim spasi depan-belakang
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
//  Fungsi bantu: membaca nama peserta (tidak boleh kosong)
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
//  Fungsi bantu: konversi string ke integer murni
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
//  Fungsi bantu: membaca karakter Y atau T (case insensitive)
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
//  Fungsi bantu: membaca bilangan bulat positif (>0)
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
//  Fungsi bantu: membaca pilihan kategori (1=Mahasiswa, 2=Dosen, 3=Umum)
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
//  Fungsi bantu: konversi id kategori ke nama teks
// ================================================================
string namaKategori(int kategori) {
    if (kategori == 1) return "Mahasiswa";
    if (kategori == 2) return "Dosen";
    return "Umum";
}

// ================================================================
//  Fungsi bantu: mengembalikan tarif dasar berdasarkan kategori
// ================================================================
double tarifDasar(int kategori) {
    if (kategori == 1) return TARIF_MAHASISWA;
    if (kategori == 2) return TARIF_DOSEN;
    return TARIF_UMUM;
}

// ================================================================
//  Fungsi bantu: cetak garis pemisah
// ================================================================
void cetakGaris(char karakter, int panjang) {
    for (int i = 0; i < panjang; i++) cout << karakter;
    cout << "\n";
}

// ================================================================
//  Fungsi bantu: format rupiah (contoh: 175000 -> "Rp175.000")
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
//  Fungsi bantu: format persentase (contoh: 33.333 -> "33.3%")
// ================================================================
string formatPersen(double persentase) {
    ostringstream ss;
    ss << fixed << setprecision(1) << persentase << "%";
    return ss.str();
}

// ================================================================
//  Fungsi bantu: evaluasi kategori peserta terbanyak
// ================================================================
string evaluasiKategoriTerbanyak(int jmlMhs, int jmlDosen, int jmlUmum) {
    if (jmlMhs == 0 && jmlDosen == 0 && jmlUmum == 0) {
        return "Belum ada data pendaftar";
    }

    // Kasus 1: Ketiga kategori memiliki jumlah yang sama
    if (jmlMhs == jmlDosen && jmlDosen == jmlUmum) {
        return "Ketiga kategori seimbang (" + to_string(jmlMhs) + " peserta tiap kategori)";
    }

    // Kasus 2: Dua kategori imbang tertinggi
    if (jmlMhs == jmlDosen && jmlMhs > jmlUmum) {
        return "Mahasiswa & Dosen (Imbang terbanyak: " + to_string(jmlMhs) + " peserta)";
    }
    if (jmlMhs == jmlUmum && jmlMhs > jmlDosen) {
        return "Mahasiswa & Umum (Imbang terbanyak: " + to_string(jmlMhs) + " peserta)";
    }
    if (jmlDosen == jmlUmum && jmlDosen > jmlMhs) {
        return "Dosen & Umum (Imbang terbanyak: " + to_string(jmlDosen) + " peserta)";
    }

    // Kasus 3: Tepat satu kategori tunggal tertinggi
    if (jmlMhs > jmlDosen && jmlMhs > jmlUmum) {
        return "Mahasiswa (" + to_string(jmlMhs) + " peserta)";
    }
    if (jmlDosen > jmlMhs && jmlDosen > jmlUmum) {
        return "Dosen (" + to_string(jmlDosen) + " peserta)";
    }
    return "Umum (" + to_string(jmlUmum) + " peserta)";
}

// ================================================================
//  PROGRAM UTAMA - MODUL B
// ================================================================
int main() {
    // ---- 1. Inisialisasi Variabel Akumulator Rekapitulasi ----
    int    totalPesertaMahasiswa  = 0;
    int    totalPesertaDosen      = 0;
    int    totalPesertaUmum       = 0;
    int    totalEarlyBird         = 0;
    int    totalKelompokDiskon    = 0;
    int    totalSertifikat        = 0;
    int    totalMakan             = 0;

    double totalBruto             = 0.0;
    double totalDiskonEB          = 0.0;
    double totalDiskonKelompok    = 0.0;
    double totalDiskon            = 0.0;
    double totalBiayaSertifikat   = 0.0;
    double totalBiayaMakan        = 0.0;
    double totalPendapatan        = 0.0;

    // Pelacak transaksi tertinggi
    double tagihanTerbesar        = 0.0;
    string namaPesertaTerbesar    = "-";
    int    nomorUrutTerbesar      = 0;

    int    nomorUrut              = 1;
    char   lanjut                 = 'Y';

    // ---- Header Program ----
    cout << "\n";
    cetakGaris('=', 64);
    cout << "   MODUL B - REKAPITULASI PENDAFTARAN SEMINAR NASIONAL\n";
    cout << "   Studi Kasus 07 | SSI1144 Algoritma dan Pengantar Pemrograman\n";
    cout << "   Penanggung Jawab : Praysilia Sifra (Modul B)\n";
    cetakGaris('=', 64);

    // ================================================================
    //  PERULANGAN INPUT DATA TRANSAKSI
    // ================================================================
    while (lanjut == 'Y') {
        cout << "\n";
        cetakGaris('-', 64);
        cout << "  INPUT TRANSAKSI #" << nomorUrut << "\n";
        cetakGaris('-', 64);

        // A. Input Data Peserta
        string namaPeserta   = bacaNama("  Nama peserta               : ");
        int    kategori      = bacaKategori();
        char   statusEB      = bacaYaaTidak("  Status Early Bird? (Y/T)   : ");
        bool   earlyBird     = (statusEB == 'Y');
        int    jumlahAnggota = bacaIntPositif("  Jumlah anggota rombongan   : ");
        bool   kelompok      = (jumlahAnggota >= MIN_ANGGOTA_KELOMPOK);
        char   statusSertif  = bacaYaaTidak("  Pesan sertifikat? (Y/T)    : ");
        bool   pesanSertif   = (statusSertif == 'Y');
        char   statusMakan   = bacaYaaTidak("  Pesan paket makan? (Y/T)   : ");
        bool   pesanMakan    = (statusMakan == 'Y');

        // B. Perhitungan Tagihan Per Peserta
        double tarif = tarifDasar(kategori);

        double nominalEB = 0.0;
        if (earlyBird) {
            nominalEB = tarif * DISKON_EARLY_BIRD;
        }
        double tarifSetelahEB = tarif - nominalEB;

        double nominalKlp = 0.0;
        if (kelompok) {
            nominalKlp = tarifSetelahEB * DISKON_KELOMPOK;
        }
        double tarifBersih = tarifSetelahEB - nominalKlp;

        double biayaTambahan = 0.0;
        if (pesanSertif) biayaTambahan += BIAYA_SERTIFIKAT;
        if (pesanMakan)  biayaTambahan += BIAYA_MAKAN;

        double diskonPeserta = nominalEB + nominalKlp;
        double tagihanPeserta = tarifBersih + biayaTambahan;

        // C. Akumulasi Data ke Variabel Rekapitulasi (Modul B)
        if (kategori == 1)      totalPesertaMahasiswa++;
        else if (kategori == 2) totalPesertaDosen++;
        else                    totalPesertaUmum++;

        if (earlyBird)   totalEarlyBird++;
        if (kelompok)    totalKelompokDiskon++;
        if (pesanSertif) {
            totalSertifikat++;
            totalBiayaSertifikat += BIAYA_SERTIFIKAT;
        }
        if (pesanMakan) {
            totalMakan++;
            totalBiayaMakan += BIAYA_MAKAN;
        }

        totalBruto          += (tarif + biayaTambahan);
        totalDiskonEB       += nominalEB;
        totalDiskonKelompok += nominalKlp;
        totalDiskon         += diskonPeserta;
        totalPendapatan     += tagihanPeserta;

        // Cek rekor transaksi terbesar
        if (tagihanPeserta > tagihanTerbesar) {
            tagihanTerbesar     = tagihanPeserta;
            namaPesertaTerbesar = namaPeserta;
            nomorUrutTerbesar   = nomorUrut;
        }

        // D. Konfirmasi Pencatatan Transaksi
        cout << "\n  [OK] Data Pendaftaran #" << nomorUrut << " (" << namaPeserta 
             << " - " << namaKategori(kategori) << ") berhasil dicatat.\n";
        cout << "       Tagihan: " << formatRupiah(tagihanPeserta)
             << " | Potongan Diskon: " << formatRupiah(diskonPeserta) << "\n";

        nomorUrut++;
        cout << "\n";
        lanjut = bacaYaaTidak("  Input transaksi pendaftaran berikutnya? (Y/T) : ");
    }

    // ================================================================
    //  LAPORAN REKAPITULASI AKHIR (MODUL B)
    // ================================================================
    int totalPeserta = totalPesertaMahasiswa + totalPesertaDosen + totalPesertaUmum;

    cout << "\n\n";
    cetakGaris('=', 64);
    cout << "        LAPORAN REKAPITULASI KEGIATAN SEMINAR NASIONAL\n";
    cout << "                  PRODUK KELUARAN MODUL B\n";
    cetakGaris('=', 64);

    if (totalPeserta == 0) {
        cout << "\n  [i] Tidak ada transaksi pendaftaran yang dicatat pada sesi ini.\n";
        cout << "      Rekapitulasi tidak dapat dihitung.\n\n";
        cetakGaris('=', 64);
        return 0;
    }

    // Perhitungan Persentase Kategori
    double persenMhs  = ((double)totalPesertaMahasiswa / totalPeserta) * 100.0;
    double persenDsn  = ((double)totalPesertaDosen / totalPeserta) * 100.0;
    double persenUmum = ((double)totalPesertaUmum / totalPeserta) * 100.0;

    // Perhitungan Rata-rata
    double rataRataPendapatan = totalPendapatan / totalPeserta;
    double rataRataDiskon     = totalDiskon / totalPeserta;

    // Evaluasi Kategori Terbanyak
    string kategoriTerbanyak = evaluasiKategoriTerbanyak(totalPesertaMahasiswa, 
                                                         totalPesertaDosen, 
                                                         totalPesertaUmum);

    // 1. REKAPITULASI PESERTA & KATEGORI
    cout << "\n  [1] REKAPITULASI JUMLAH PESERTA BERDASARKAN KATEGORI\n";
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

    // 2. REKAPITULASI BENEFIT & LAYANAN TAMBAHAN
    cout << "\n  [2] REKAPITULASI LAYANAN DAN BENEFIT TAMBAHAN\n";
    cetakGaris('-', 64);
    cout << "  - Peserta Early Bird (Diskon 20%)  : " << setw(5) << totalEarlyBird 
         << " orang (" << formatPersen(((double)totalEarlyBird / totalPeserta) * 100.0) << ")\n";
    cout << "  - Rombongan Diskon Kelompok (>=5)  : " << setw(5) << totalKelompokDiskon 
         << " transaksi\n";
    cout << "  - Pemesan Sertifikat Cetak fisik   : " << setw(5) << totalSertifikat 
         << " orang (Total: " << formatRupiah(totalBiayaSertifikat) << ")\n";
    cout << "  - Pemesan Paket Makan / Konsumsi   : " << setw(5) << totalMakan 
         << " orang (Total: " << formatRupiah(totalBiayaMakan) << ")\n";

    // 3. REKAPITULASI KEUANGAN & PENDAPATAN
    cout << "\n  [3] REKAPITULASI KEUANGAN DAN ARUS PENDAPATAN\n";
    cetakGaris('-', 64);
    cout << right;
    cout << "  Total Nilai Transaksi Kotor (Bruto) : " << setw(15) << formatRupiah(totalBruto) << "\n";
    cout << "  Total Potongan Diskon Early Bird    : " << setw(15) << ("-" + formatRupiah(totalDiskonEB)) << "\n";
    cout << "  Total Potongan Diskon Kelompok      : " << setw(15) << ("-" + formatRupiah(totalDiskonKelompok)) << "\n";
    cetakGaris('-', 64);
    cout << "  TOTAL AKUMULASI DISKON DIKELUARKAN  : " << setw(15) << ("-" + formatRupiah(totalDiskon)) << "\n";
    cout << "  TOTAL PENDAPATAN BERSIH KEGIATAN    : " << setw(15) << formatRupiah(totalPendapatan) << "\n";
    cetakGaris('=', 64);

    // 4. METRIK STATISTIK TAMBAHAN
    cout << left;
    cout << "\n  [4] STATISTIK & METRIK ANALISIS\n";
    cetakGaris('-', 64);
    cout << "  - Rata-rata Pendapatan per Peserta  : " << formatRupiah(rataRataPendapatan) << " / orang\n";
    cout << "  - Rata-rata Diskon per Peserta      : " << formatRupiah(rataRataDiskon) << " / orang\n";
    cout << "  - Transaksi Terbesar                : " << formatRupiah(tagihanTerbesar) << "\n";
    cout << "    (Dicatat oleh Peserta #" << nomorUrutTerbesar << " - " << namaPesertaTerbesar << ")\n";
    cetakGaris('=', 64);

    cout << "\n  Sesi pelaporan rekapitulasi Modul B selesai dengan sukses.\n\n";
    return 0;
}

