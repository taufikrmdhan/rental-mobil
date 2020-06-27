#include <iostream>
#include <ctime>
#include <cstdio>
#include <string.h>
#include <stdlib.h>

using namespace std;

struct _jenis_mobil {
	string nama;
	unsigned int harga;
};

struct row_peminjaman {
	uint32_t id;
	char nama[64];
	char nomor_telepon[15];
	char alamat[64];
	uint8_t jangka_kontrak;
	time_t waktu_peminjaman;
	int jenis_mobil;
};

const static struct _jenis_mobil jenis_mobil[] = {
	{"AVANZA", 100000},
	{"MOBILIO", 100000},
	{"PAJERO SPORT", 250000},
	{"BUGATI", 500000},
	{"FERRARI", 500000}
};

const static string bulan [] = {
	"Januari", "februari", "maret", "april", "mei", "juni", "juli", 
	"agustus", "september", "oktober", "november", "desember"
};

uint32_t ambil_id_terakhir();
void tampilkan_jenis_mobil();
void peminjaman_jenis_mobil();
void pengembalian_mobil();
bool pencarian_data(struct row_peminjaman *row, uint32_t id);

int main()
{
	int pilih;
	balik:
	cout << "==================================================\n";
	cout << "**********SELAMAT DATANG DI KINCAI SORUM**********\n";
	cout << "==================================================\n";
	pilihan:
	cout << "*\nMasukkan menu pilihan anda*\n";
	cout << "\n1.Jenis Mobil\n";
	cout << "\n2.Peminjaman Mobil\n";
	cout << "\n3.Pengembalian Mobil\n";
	cout << "\n4.Keluar dari program\n";
	cin>>pilih;
	system("cls");
	switch (pilih) {
		case 1:
			tampilkan_jenis_mobil();
			goto balik;
		case 2:
			peminjaman_jenis_mobil();
			goto balik;
		case 3:
			pengembalian_mobil();
			goto balik;
		case 4:
			return (0);
			break;
		default:
			goto pilihan;
			break;

	}
}

void tampilkan_jenis_mobil()
{
	cout << "=======Jenis mobil yang tersedia======" << endl;
	for (int i=0; i<5; i++){
		cout << i+1 << "." << jenis_mobil[i].nama << endl;
	}
}


void peminjaman_jenis_mobil()
{
	FILE *table;
	unsigned int x;
	char konfirm;
	uint32_t tarif_pinjam;
	struct row_peminjaman row;
	time_t asd;
	struct tm *qwe;

	time(&asd);
	qwe = localtime(&asd);

	cout << "\n=======peminjaman========\n" << endl;
	cout << "Tampilan harga sewa mobil\n";
	for (int i = 0; i < 5; ++i) {
		cout << "\n"<< (1 + i) << "." << jenis_mobil[i].nama << "\n" << "(/hari  " << "=" << "Rp." << jenis_mobil[i].harga << ")" << "\n";
	}

	konfirmasi:
	cout << "\n\nApakah anda ingin melanjutkan? [y/n] :\n";
	cin>>konfirm;
	if (konfirm=='y' || konfirm=='Y'){
		cout << "Silahkan pilih jenis mobil: ";
		cin>>row.jenis_mobil;
		if (row.jenis_mobil>=1 && row.jenis_mobil<=5){
			cin.ignore();
			cout << "\nMasukkan data diri anda\n" << endl;

			cout << "Nama\t\t\t: ";
			fgets(row.nama, 64, stdin);
			row.nama[strlen(row.nama) - 1] = '\0';

			cout << "Nomor telepon\t\t: ";
			fgets(row.nomor_telepon, 15, stdin);
			row.nomor_telepon[strlen(row.nomor_telepon) - 1] = '\0';
			
			cout << "Alamat\t\t\t: ";
			fgets(row.alamat, 64, stdin);
			row.alamat[strlen(row.alamat) - 1] = '\0'; 
			
			cout << "Jangka Kontrak\t\t: ";
			cin>>x;
			row.jangka_kontrak = (uint8_t)x;
			cin.ignore();

			cout << "\n\nDetail peminjaman anda\n\n";
			cout << "Nama\t\t\t: " << row.nama << endl;
			cout << "Nomor telepon\t\t: " << row.nomor_telepon << endl;
			cout << "Alamat\t\t\t: " << row.alamat << endl;
			cout << "Tanggal sekarang\t: " << qwe->tm_mday << " " <<bulan[qwe->tm_mon] <<" " << qwe-> tm_year+1900 << endl;
			cout << "Jangka Kontrak\t\t: " << ((uint32_t)row.jangka_kontrak) << endl;

			row.waktu_peminjaman = asd;
			tarif_pinjam = jenis_mobil[row.jenis_mobil-1].harga * row.jangka_kontrak;

			cout << "Harga sewa\t\t: " << tarif_pinjam << endl;
			cout << "Mobil yang anda pilih\t: " << jenis_mobil[row.jenis_mobil-1].nama << endl;

			cout << "\nApakah data tersebut sudah benar? [y/n] ";
			cin >> konfirm;
			if (konfirm == 'Y' || konfirm == 'y') {
				row.id = ambil_id_terakhir() + 1;

				table = fopen("database.dat", "ab+");
				fwrite(&row, sizeof(row), 1, table);
				fclose(table);

				cout << "\nData berhasil disimpan!\n";
				cout << "Kode peminjaman Anda adalah ";
				printf("%04d", row.id);
				cout << "\n\nSilahkan berikan KTP anda sebagai jaminan peminjaman."<<endl;
			} else {
				cout << "\n\n";
				goto konfirmasi;
			}
		}
	} else if (konfirm=='n' || konfirm=='N'){
		cout << "Terima kasih telah berkunjung";
		exit (0);
	} else {
		goto konfirmasi;
	}
}

uint32_t ambil_id_terakhir()
{
	struct row_peminjaman tmp;
	memset(&tmp, '\0', sizeof(tmp));
	FILE *table = fopen("database.dat", "rb");
	if (table == NULL) {
		return 0;
	} else {
		while (!feof(table)) {
			fread(&tmp, sizeof(tmp), 1, table);
		}
		return tmp.id;
	}
}

void pengembalian_mobil()
{
	char buffer[10];
	struct row_peminjaman row;
	time_t asd, selisih;
	struct tm *qwe;
	int denda;

	time(&asd);
	qwe = localtime(&asd);

	cin.ignore();
	cout << "======Pengembalian mobil=======\n";
	cout << "Masukkan kode peminjaman Anda: ";
	fgets(buffer, 10, stdin);
	buffer[strlen(buffer) - 1] = '\0';
	row.id = atoi(buffer);
	if (pencarian_data(&row, row.id)) {
		cout << "Data ditemukan!\n";
		cout << "\n\nDetail pengembalian anda\n\n";
		cout << "Nama\t\t\t: " << row.nama << endl;
		cout << "Nomor telepon\t\t: " << row.nomor_telepon << endl;
		cout << "Alamat\t\t\t: " << row.alamat << endl;
		cout << "Tanggal sekarang\t: " << qwe->tm_mday << " " <<bulan[qwe->tm_mon] <<" " << qwe-> tm_year+1900 << endl;

		qwe = localtime(&(row.waktu_peminjaman));
		cout << "Tanggal pinjam\t\t: " << qwe->tm_mday << " " <<bulan[qwe->tm_mon] <<" " << qwe-> tm_year+1900 << endl;
		selisih = ((asd - row.waktu_peminjaman) / (3600 * 24)) + 1;

		cout << "Jangka Kontrak\t\t: " << ((uint32_t)row.jangka_kontrak) << endl;
		cout << "Waktu peminjaman\t\t: " << selisih << " hari" << endl;
		denda = ((((int)selisih - (int)row.jangka_kontrak)) * 50000);
		denda = denda < 0 ? 0 : denda;
		cout << "Denda\t\t\t: " << denda << endl;
		cout<< "================================"<<endl;
		cout<< "Terima kasih telah melakukan peminjaman";
	} else {
		cout << "Data dengan kode peminjaman " << buffer << " tidak ditemukan!";
	}
}

bool pencarian_data(struct row_peminjaman *row, uint32_t id)
{
	struct row_peminjaman tmp;
	FILE *table = fopen("database.dat", "rb");
	while (!feof(table)) {
		fread(&tmp, sizeof(tmp), 1, table);
		if (tmp.id == id) {
			*row = tmp;
			fclose(table);
			return true;
			break;
		}
	}
	fclose(table);
	return false;
}
