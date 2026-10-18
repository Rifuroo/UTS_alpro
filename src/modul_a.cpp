/*
 * =============================================================
 *  MODUL A - Sistem Pendaftaran Seminar Nasional
 *  Studi Kasus 07 | SSI1144 Algoritma dan Pengantar Pemrograman
 *  Program Studi Sistem Informasi - Universitas Ma Chung
 * =============================================================
 *
 *  Ketentuan biaya:
 *    - Tarif dasar : Mahasiswa Rp75.000 | Dosen Rp125.000 | Umum Rp175.000
 *    - Diskon early bird  : 20% dari tarif dasar
 *    - Diskon kelompok    : 10% dari tarif setelah early bird
 *                           (berlaku jika jumlah anggota >= 5)
 *    - Sertifikat cetak   : Rp20.000 (tidak terkena diskon)
 *    - Paket makan        : Rp35.000 (tidak terkena diskon)
 *
 *  Validasi input:
 *    - Nama peserta tidak boleh kosong
 *    - Kategori harus 1 (mahasiswa), 2 (dosen), atau 3 (umum)
 *    - Status early bird : Y atau T
 *    - Jumlah anggota kelompok : minimal 1
 *    - Pilihan sertifikat dan makan : Y atau T
 *    - Pilihan lanjut : Y atau T
 *
 *  Catatan implementasi:
 *    Semua input dibaca sebagai string (getline), kemudian dikonversi
 *    sesuai tipe yang diperlukan. Ini menghindari konflik antara
 *    cin >> dan getline yang sering menyebabkan bug di C++.
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
//  Fungsi bantu: baca satu baris, trim spasi depan-belakang
// ================================================================
string bacaBaris(const string &pertanyaan) {
    string baris;
    cout << pertanyaan;
    getline(cin, baris);
    // Trim spasi kanan
    while (!baris.empty() && (baris.back() == ' ' || baris.back() == '\r')) {
        baris.pop_back();
    }
    // Trim spasi kiri
    size_t mulai = baris.find_first_not_of(' ');
    if (mulai == string::npos) return "";
    return baris.substr(mulai);
}

// ================================================================
//  Fungsi bantu: membaca nama (tidak boleh kosong)
// ================================================================
string bacaNama(const string &pertanyaan) {
    string nama;
    while (true) {
        nama = bacaBaris(pertanyaan);
        if (!nama.empty()) return nama;
        cout << "  [!] Nama tidak boleh kosong. Coba lagi.\n";
    }
}

// ================================================================
//  Fungsi bantu: membaca karakter Y/T (tidak case sensitive)
// ================================================================
char bacaYaaTidak(const string &pertanyaan) {
    while (true) {
        string baris = bacaBaris(pertanyaan);
        if (baris.length() == 1) {
            char c = toupper(baris[0]);
            if (c == 'Y' || c == 'T') return c;
        }
        cout << "  [!] Input tidak valid. Ketik Y atau T.\n";
    }
}

// ================================================================
//  Fungsi bantu: membaca integer dari string
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
//  Fungsi bantu: membaca integer positif dengan validasi
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
//  Fungsi bantu: membaca kategori (1/2/3) dengan validasi
// ================================================================
int bacaKategori() {
    while (true) {
        string baris = bacaBaris("  Kategori (1=Mahasiswa, 2=Dosen, 3=Umum) : ");
        int nilai;
        if (konversiInt(baris, nilai) && (nilai == 1 || nilai == 2 || nilai == 3)) {
            return nilai;
        }
        cout << "  [!] Pilihan tidak valid. Ketik 1, 2, atau 3.\n";
    }
}

// ================================================================
//  Fungsi bantu: nama kategori
// ================================================================
string namaKategori(int kategori) {
    if (kategori == 1) return "Mahasiswa";
    if (kategori == 2) return "Dosen";
    return "Umum";
}

// ================================================================
//  Fungsi bantu: tarif dasar berdasarkan kategori
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
//  Fungsi bantu: format rupiah (contoh: 109000 -> "Rp109.000")
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
//  PROGRAM UTAMA
// ================================================================
int main() {

    // ---- Variabel akumulator untuk rekapitulasi ----
    int    totalPesertaMahasiswa  = 0;
    int    totalPesertaDosen      = 0;
    int    totalPesertaUmum       = 0;
    int    totalEarlyBird         = 0;
    int    totalSertifikat        = 0;
    int    totalMakan             = 0;
    double totalDiskon            = 0.0;
    double totalPendapatan        = 0.0;
    int    nomorUrut              = 1;

    // ---- Header program ----
    cout << "\n";
    cetakGaris('=', 60);
    cout << "   SISTEM PENDAFTARAN SEMINAR NASIONAL\n";
    cout << "   Studi Kasus 07 | SSI1144 | Universitas Ma Chung\n";
    cetakGaris('=', 60);

    // ================================================================
    //  PERULANGAN UTAMA: proses setiap pendaftaran
    // ================================================================
    char lanjut = 'Y';

    while (lanjut == 'Y') {

        cout << "\n";
        cetakGaris('-', 60);
        cout << "  PENDAFTARAN #" << nomorUrut << "\n";
        cetakGaris('-', 60);

        // ---- 1. Input data peserta ----

        string namaPeserta   = bacaNama("  Nama peserta            : ");
        int    kategori      = bacaKategori();
        char   statusEB      = bacaYaaTidak("  Early bird? (Y/T)       : ");
        bool   earlyBird     = (statusEB == 'Y');
        int    jumlahAnggota = bacaIntPositif("  Jumlah anggota kelompok : ");
        bool   kelompok      = (jumlahAnggota >= MIN_ANGGOTA_KELOMPOK);
        char   statusSertif  = bacaYaaTidak("  Sertifikat cetak? (Y/T) : ");
        bool   pesanSertifikat = (statusSertif == 'Y');
        char   statusMakan   = bacaYaaTidak("  Paket makan? (Y/T)      : ");
        bool   pesanMakan    = (statusMakan == 'Y');

        // ---- 2. Proses perhitungan biaya ----

        double tarif = tarifDasar(kategori);

        // Diskon early bird: 20% dari tarif dasar
        double nominalDiskonEB = 0.0;
        if (earlyBird) {
            nominalDiskonEB = tarif * DISKON_EARLY_BIRD;
        }
        double tarifSetelahEB = tarif - nominalDiskonEB;

        // Diskon kelompok: 10% dari tarif setelah EB, syarat >= 5 anggota
        double nominalDiskonKelompok = 0.0;
        if (kelompok) {
            nominalDiskonKelompok = tarifSetelahEB * DISKON_KELOMPOK;
        }
        double tarifBersih = tarifSetelahEB - nominalDiskonKelompok;

        // Total diskon peserta ini
        double totalDiskonPeserta = nominalDiskonEB + nominalDiskonKelompok;

        // Biaya tambahan: tidak terkena diskon
        double biayaTambahan = 0.0;
        if (pesanSertifikat) biayaTambahan += BIAYA_SERTIFIKAT;
        if (pesanMakan)      biayaTambahan += BIAYA_MAKAN;

        // Total tagihan peserta ini
        double totalTagihan = tarifBersih + biayaTambahan;

        // ---- 3. Akumulasi data ----
        if      (kategori == 1) totalPesertaMahasiswa++;
        else if (kategori == 2) totalPesertaDosen++;
        else                    totalPesertaUmum++;

        if (earlyBird)       totalEarlyBird++;
        if (pesanSertifikat) totalSertifikat++;
        if (pesanMakan)      totalMakan++;

        totalDiskon     += totalDiskonPeserta;
        totalPendapatan += totalTagihan;

        // ---- 4. Output: rincian tagihan ----
        cout << "\n";
        cetakGaris('=', 60);
        cout << "  RINCIAN TAGIHAN - PENDAFTARAN #" << nomorUrut << "\n";
        cetakGaris('=', 60);
        cout << left;
        cout << "  Nama peserta            : " << namaPeserta << "\n";
        cout << "  Kategori                : " << namaKategori(kategori) << "\n";
        cout << "  Jumlah anggota kelompok : " << jumlahAnggota << " orang"
             << (kelompok ? " (diskon kelompok berlaku)" : " (tidak memenuhi diskon kelompok)") << "\n";
        cout << "  Status early bird       : " << (earlyBird ? "Ya (diskon 20%)" : "Tidak") << "\n";
        cetakGaris('-', 60);
        cout << right;
        cout << "  Tarif dasar             : " << setw(12) << formatRupiah(tarif) << "\n";

        if (earlyBird) {
            cout << "  Diskon early bird (20%) : " << setw(11) << ("-" + formatRupiah(nominalDiskonEB)) << "\n";
            cout << "  Tarif setelah EB        : " << setw(12) << formatRupiah(tarifSetelahEB) << "\n";
        }

        if (kelompok) {
            cout << "  Diskon kelompok (10%)   : " << setw(11) << ("-" + formatRupiah(nominalDiskonKelompok)) << "\n";
        }

        cout << "  Tarif bersih            : " << setw(12) << formatRupiah(tarifBersih) << "\n";

        if (pesanSertifikat || pesanMakan) {
            cetakGaris('-', 60);
            cout << left << "  Biaya tambahan (tanpa diskon):\n" << right;
            if (pesanSertifikat) {
                cout << "    Sertifikat cetak      : " << setw(12) << formatRupiah(BIAYA_SERTIFIKAT) << "\n";
            }
            if (pesanMakan) {
                cout << "    Paket makan           : " << setw(12) << formatRupiah(BIAYA_MAKAN) << "\n";
            }
        }

        cetakGaris('-', 60);
        cout << "  TOTAL DISKON DIBERIKAN  : " << setw(11) << ("-" + formatRupiah(totalDiskonPeserta)) << "\n";
        cout << "  TOTAL TAGIHAN           : " << setw(12) << formatRupiah(totalTagihan) << "\n";
        cetakGaris('=', 60);

        // ---- 5. Tanya lanjut ----
        nomorUrut++;
        cout << "\n";
        lanjut = bacaYaaTidak("  Daftarkan peserta lain? (Y/T) : ");
    }

    // ================================================================
    //  REKAPITULASI AKHIR SESI
    // ================================================================
    int totalPeserta = totalPesertaMahasiswa + totalPesertaDosen + totalPesertaUmum;

    cout << "\n";
    cetakGaris('=', 60);
    cout << "  REKAPITULASI SESI - MODUL A\n";
    cetakGaris('=', 60);
    cout << left;
    cout << "  Total pendaftar          : " << totalPeserta << " orang\n";
    cout << "    - Mahasiswa            : " << totalPesertaMahasiswa << " orang\n";
    cout << "    - Dosen                : " << totalPesertaDosen << " orang\n";
    cout << "    - Umum                 : " << totalPesertaUmum << " orang\n";
    cetakGaris('-', 60);
    cout << "  Peserta early bird       : " << totalEarlyBird << " orang\n";
    cout << "  Pemesan sertifikat cetak : " << totalSertifikat << " orang\n";
    cout << "  Pemesan paket makan      : " << totalMakan << " orang\n";
    cetakGaris('-', 60);
    cout << "  Total diskon diberikan   : " << formatRupiah(totalDiskon) << "\n";
    cout << "  Total pendapatan sesi    : " << formatRupiah(totalPendapatan) << "\n";
    cetakGaris('=', 60);
    cout << "\n  Program selesai. Terima kasih.\n\n";

    return 0;
}
