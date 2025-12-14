#include "LibrarySystem.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <algorithm>
#include <ctime>
#include <cctype>
#include <limits>

using namespace std;

// ===========================================================
// Helper Date Functions
// ===========================================================
string getCurrentDate() {
    time_t now = time(nullptr);
    tm *t = localtime(&now);
    stringstream ss;
    ss << setw(2) << setfill('0') << t->tm_mday << "/"
       << setw(2) << setfill('0') << (t->tm_mon + 1) << "/"
       << (t->tm_year + 1900);
    return ss.str();
}

string addDays(const string &date, int days) {
    int d, m, y;
    char s;
    stringstream ss(date);
    ss >> d >> s >> m >> s >> y;

    int daysInMonth[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    
    if ((y % 4 == 0 && y % 100 != 0) || (y % 400 == 0)) {
        daysInMonth[1] = 29;
    }

    d += days;
    while (d > daysInMonth[m - 1]) {
        d -= daysInMonth[m - 1];
        m++;
        if (m > 12) {
            m = 1;
            y++;
            if ((y % 4 == 0 && y % 100 != 0) || (y % 400 == 0)) {
                daysInMonth[1] = 29;
            } else {
                daysInMonth[1] = 28;
            }
        }
    }

    stringstream out;
    out << setw(2) << setfill('0') << d << "/"
        << setw(2) << setfill('0') << m << "/" << y;
    return out.str();
}

int daysDifference(const string &date1, const string &date2) {
    int d1, m1, y1, d2, m2, y2;
    char s;
    stringstream ss1(date1), ss2(date2);
    ss1 >> d1 >> s >> m1 >> s >> y1;
    ss2 >> d2 >> s >> m2 >> s >> y2;

    int daysInMonth[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    
    int total1 = y1 * 365 + d1;
    for (int i = 0; i < m1 - 1; i++) total1 += daysInMonth[i];
    total1 += y1 / 4;
    
    int total2 = y2 * 365 + d2;
    for (int i = 0; i < m2 - 1; i++) total2 += daysInMonth[i];
    total2 += y2 / 4;
    
    return total2 - total1;
}

int getValidInt(const string& prompt) {
    int value;
    while (true) {
        cout << prompt;
        if (cin >> value && value >= 0) {
            return value;
        }
        cout << "[ERROR] Masukkan angka valid (>= 0)!\n";
        cin.clear();
        cin.ignore(10000, '\n');
    }
}

string toLower(string str) {
    for (char &c : str) c = tolower(c);
    return str;
}

// ===========================================================
// AUTHENTICATION - FIXED!
// ===========================================================

bool LibrarySystem::loginAsAdmin() {
    string username, password;
    
    cout << "\n====================================\n";
    cout << "         LOGIN ADMINISTRATOR        \n";
    cout << "====================================\n";
    cout << "Username: ";
    cin >> username;
    cout << "Password: ";
    cin >> password;
    
    // Hardcoded credentials untuk demo ae brader kek wifi indihome
    if (username == "admin" && password == "admin123") {
        currentUserRole = "ADMIN";
        currentUserId = "ADMIN";
        cout << "\n[OK] Login berhasil sebagai Administrator!\n";
        cout << "Selamat datang, Admin!\n";
        return true;
    }
    
    cout << "\n[ERROR] Username atau password salah!\n";
    cout << "Hint: username='admin', password='admin123'\n";
    return false;
}

bool LibrarySystem::loginAsMember() {
    string memberId;
    
    cout << "\n====================================\n";
    cout << "           LOGIN MEMBER             \n";
    cout << "====================================\n";
    cout << "Masukkan ID Member Anda: ";
    cin >> memberId;
    
    // Cek apakah member ID ada dalam database
    for (auto &m : members) {
        if (m.getId() == memberId) {
            currentUserRole = "MEMBER";
            currentUserId = memberId;
            cout << "\n[OK] Login berhasil!\n";
            cout << "Selamat datang, " << m.getName() << "!\n";
            return true;
        }
    }
    
    cout << "\n[ERROR] ID Member tidak ditemukan!\n";
    cout << "Pastikan Anda sudah terdaftar sebagai member.\n";
    return false;
}

void LibrarySystem::logout() {
    string prevRole = currentUserRole;
    currentUserRole = "";
    currentUserId = "";
    cout << "\n[OK] Logout berhasil. Terima kasih!\n";
}

// ===========================================================
// Load & Save
// ===========================================================

void LibrarySystem::loadFromFile(const string& path) {
    ifstream file(path);
    if (!file.is_open()) {
        cout << "[INFO] File " << path << " tidak ditemukan. Menggunakan data kosong.\n";
        return;
    }

    string line, section = "";
    books.clear();
    members.clear();
    transactions.clear();

    while (getline(file, line)) {
        if (line == "[BOOKS]") { section = "BOOKS"; continue; }
        if (line == "[MEMBERS]") { section = "MEMBERS"; continue; }
        if (line == "[TRANSACTIONS]") { section = "TRANSACTIONS"; continue; }
        if (line.empty()) continue;

        try {
            if (section == "BOOKS") {
                books.push_back(Book::fromFile(line));
            }
            else if (section == "MEMBERS") {
                members.push_back(Member::fromFile(line));
            }
            else if (section == "TRANSACTIONS") {
                transactions.push_back(Transaction::fromFile(line));
            }
        } catch (...) {
            cout << "[WARN] Baris error di-skip: " << line << endl;
        }
    }

    if (!books.empty()) {
        string lastId = books.back().getId();
        bookIdCounter = stoi(lastId.substr(1)) + 1;
    }
    if (!members.empty()) {
        string lastId = members.back().getId();
        memberIdCounter = stoi(lastId.substr(1)) + 1;
    }
    if (!transactions.empty()) {
        string lastId = transactions.back().getId();
        transIdCounter = stoi(lastId.substr(1)) + 1;
    }

    cout << "[OK] Data dimuat dari " << path << "\n";
    cout << "  - " << books.size() << " buku\n";
    cout << "  - " << members.size() << " anggota\n";
    cout << "  - " << transactions.size() << " transaksi\n";
}

void LibrarySystem::saveToFile(const string& path) {
    ofstream file(path);
    if (!file.is_open()) {
        cout << "[ERROR] Tidak dapat menulis ke " << path << "\n";
        return;
    }

    file << "[BOOKS]\n";
    for (auto &b : books) file << b.toFile() << "\n";

    file << "\n[MEMBERS]\n";
    for (auto &m : members) file << m.toFile() << "\n";

    file << "\n[TRANSACTIONS]\n";
    for (auto &t : transactions) file << t.toFile() << "\n";

    file.close();
    cout << "[OK] Data disimpan ke " << path << "\n";
}

// ===========================================================
// Buku
// ===========================================================

void LibrarySystem::addBook() {
    cin.ignore();
    string title, author, isbn, category;
    int stock;

    cout << "\n=== TAMBAH BUKU BARU ===\n";
    
    cout << "Judul: "; 
    getline(cin, title);
    if (title.empty()) {
        cout << "[ERROR] Judul tidak boleh kosong!\n";
        return;
    }
    
    cout << "Penulis: "; 
    getline(cin, author);
    if (author.empty()) {
        cout << "[ERROR] Penulis tidak boleh kosong!\n";
        return;
    }
    
    cout << "ISBN: "; 
    getline(cin, isbn);
    
    for (auto &b : books) {
        if (!isbn.empty() && b.getIsbn() == isbn) {
            cout << "[WARNING] ISBN sudah ada! Tetap lanjutkan? (y/n): ";
            char confirm;
            cin >> confirm;
            cin.ignore();
            if (confirm != 'y' && confirm != 'Y') {
                cout << "[INFO] Penambahan buku dibatalkan.\n";
                return;
            }
            break;
        }
    }
    
    cout << "Kategori: "; 
    getline(cin, category);
    
    stock = getValidInt("Stok: ");

    string id = "B";
    if (bookIdCounter < 10) id += "000";
    else if (bookIdCounter < 100) id += "00";
    else if (bookIdCounter < 1000) id += "0";
    id += to_string(bookIdCounter++);
    
    books.push_back(Book(id, title, author, isbn, category, stock));

    cout << "[OK] Buku ditambahkan dengan ID: " << id << endl;
}

void LibrarySystem::editBook() {
    string id;
    cout << "\nMasukkan ID buku: ";
    cin >> id;

    for (auto &b : books) {
        if (b.getId() == id) {
            cin.ignore();
            string title, author, isbn, category, temp;

            cout << "\n=== EDIT BUKU (Enter untuk skip) ===\n";
            cout << "Data saat ini:\n" << b.toString() << "\n\n";

            cout << "Judul baru: ";
            getline(cin, title);
            if (!title.empty()) b.setTitle(title);

            cout << "Penulis baru: ";
            getline(cin, author);
            if (!author.empty()) b.setAuthor(author);

            cout << "ISBN baru: ";
            getline(cin, isbn);
            if (!isbn.empty()) b.setIsbn(isbn);

            cout << "Kategori baru: ";
            getline(cin, category);
            if (!category.empty()) b.setCategory(category);

            cout << "Stok baru: ";
            getline(cin, temp);
            if (!temp.empty()) {
                try {
                    int newStock = stoi(temp);
                    if (newStock < 0) {
                        cout << "[ERROR] Stok tidak boleh negatif!\n";
                    } else {
                        int diff = newStock - b.getStock();
                        b.setStock(newStock);
                        int newAvail = b.getAvailable() + diff;
                        if (newAvail < 0) newAvail = 0;
                        if (newAvail > newStock) newAvail = newStock;
                        b.setAvailable(newAvail);
                    }
                } catch (...) {
                    cout << "[ERROR] Input stok tidak valid!\n";
                }
            }

            cout << "[OK] Buku berhasil diedit\n";
            return;
        }
    }

    cout << "[ERROR] Buku tidak ditemukan\n";
}

void LibrarySystem::deleteBook() {
    string id;
    cout << "\nMasukkan ID buku: ";
    cin >> id;

    for (int i = 0; i < books.size(); i++) {
        if (books[i].getId() == id) {
            for (auto &t : transactions) {
                if (t.getBookId() == id && t.getStatus() == "BORROWED") {
                    cout << "[ERROR] Buku sedang dipinjam! Tidak dapat dihapus.\n";
                    return;
                }
            }
            
            char confirm;
            cout << "Yakin ingin menghapus buku ini? (y/n): ";
            cin >> confirm;
            
            if (confirm == 'y' || confirm == 'Y') {
                books.erase(books.begin() + i);
                cout << "[OK] Buku berhasil dihapus\n";
            } else {
                cout << "[INFO] Penghapusan dibatalkan\n";
            }
            return;
        }
    }

    cout << "[ERROR] Buku tidak ditemukan\n";
}

void LibrarySystem::displayAllBooks() {
    if (books.empty()) {
        cout << "\n[INFO] Tidak ada buku dalam sistem.\n";
        return;
    }

    cout << "\n=== DAFTAR SEMUA BUKU ===\n";
    cout << string(120, '=') << "\n";
    cout << left << setw(8) << "ID" 
         << setw(30) << "Judul" 
         << setw(20) << "Penulis" 
         << setw(15) << "ISBN"
         << setw(15) << "Kategori"
         << setw(8) << "Stok" 
         << setw(8) << "Tersedia" << "\n";
    cout << string(120, '=') << "\n";
    
    for (auto &b : books) {
        cout << b.toString() << endl;
    }
    cout << string(120, '=') << "\n";
    cout << "Total: " << books.size() << " buku\n";
}

void LibrarySystem::searchBook() {
    cin.ignore();
    string key;
    cout << "\nMasukkan kata kunci (judul/penulis/ISBN): ";
    getline(cin, key);

    if (key.empty()) {
        cout << "[ERROR] Kata kunci tidak boleh kosong!\n";
        return;
    }

    string keyLower = toLower(key);
    bool found = false;
    
    cout << "\n=== HASIL PENCARIAN ===\n";
    cout << string(120, '=') << "\n";

    for (auto &b : books) {
        string titleLower = toLower(b.getTitle());
        string authorLower = toLower(b.getAuthor());
        string isbnLower = toLower(b.getIsbn());
        
        if (titleLower.find(keyLower) != string::npos ||
            authorLower.find(keyLower) != string::npos ||
            isbnLower.find(keyLower) != string::npos) {
            
            if (!found) {
                cout << left << setw(8) << "ID" 
                     << setw(30) << "Judul" 
                     << setw(20) << "Penulis" 
                     << setw(15) << "ISBN"
                     << setw(15) << "Kategori"
                     << setw(8) << "Stok" 
                     << setw(8) << "Tersedia" << "\n";
                cout << string(120, '=') << "\n";
            }
            cout << b.toString() << endl;
            found = true;
        }
    }

    if (!found) {
        cout << "[INFO] Tidak ada buku yang cocok dengan kata kunci \"" << key << "\"\n";
    } else {
        cout << string(120, '=') << "\n";
    }
}

// ===========================================================
// Anggota
// ===========================================================

void LibrarySystem::addMember() {
    cin.ignore();
    string name, email, phone, address;

    cout << "\n=== TAMBAH ANGGOTA BARU ===\n";
    
    cout << "Nama Lengkap: ";
    getline(cin, name);
    if (name.empty()) {
        cout << "[ERROR] Nama tidak boleh kosong!\n";
        return;
    }
    
    cout << "Email: ";
    getline(cin, email);
    
    cout << "Telepon: ";
    getline(cin, phone);
    
    cout << "Alamat: ";
    getline(cin, address);

    string id = "M";
    if (memberIdCounter < 10) id += "000";
    else if (memberIdCounter < 100) id += "00";
    else if (memberIdCounter < 1000) id += "0";
    id += to_string(memberIdCounter++);
    
    string joinDate = getCurrentDate();
    members.push_back(Member(id, name, email, phone, address, joinDate));

    cout << "\n[OK] Anggota ditambahkan dengan ID: " << id << endl;
    cout << "PENTING: Catat ID ini untuk login sebagai member!\n";
}

void LibrarySystem::editMember() {
    string id;
    cout << "\nMasukkan ID anggota: ";
    cin >> id;

    for (auto &m : members) {
        if (m.getId() == id) {
            cin.ignore();
            string name, email, phone, addr;

            cout << "\n=== EDIT ANGGOTA (Enter untuk skip) ===\n";
            cout << "Data saat ini:\n" << m.toString() << "\n\n";

            cout << "Nama baru: "; 
            getline(cin, name);
            if (!name.empty()) m.setName(name);

            cout << "Email baru: "; 
            getline(cin, email);
            if (!email.empty()) m.setEmail(email);

            cout << "Telepon baru: "; 
            getline(cin, phone);
            if (!phone.empty()) m.setPhone(phone);

            cout << "Alamat baru: "; 
            getline(cin, addr);
            if (!addr.empty()) m.setAddress(addr);

            cout << "[OK] Data anggota diperbarui\n";
            return;
        }
    }

    cout << "[ERROR] Anggota tidak ditemukan\n";
}

void LibrarySystem::deleteMember() {
    string id;
    cout << "\nMasukkan ID anggota: ";
    cin >> id;

    for (int i = 0; i < members.size(); i++) {
        if (members[i].getId() == id) {
            for (auto &t : transactions) {
                if (t.getMemberId() == id && t.getStatus() == "BORROWED") {
                    cout << "[ERROR] Anggota masih memiliki peminjaman aktif!\n";
                    return;
                }
            }
            
            char confirm;
            cout << "Yakin ingin menghapus anggota ini? (y/n): ";
            cin >> confirm;
            
            if (confirm == 'y' || confirm == 'Y') {
                members.erase(members.begin() + i);
                cout << "[OK] Anggota dihapus\n";
            } else {
                cout << "[INFO] Penghapusan dibatalkan\n";
            }
            return;
        }
    }

    cout << "[ERROR] Anggota tidak ditemukan\n";
}

void LibrarySystem::displayAllMembers() {
    if (members.empty()) {
        cout << "\n[INFO] Tidak ada anggota dalam sistem.\n";
        return;
    }

    cout << "\n=== DAFTAR SEMUA ANGGOTA ===\n";
    cout << string(120, '=') << "\n";
    cout << left << setw(8) << "ID" 
         << setw(25) << "Nama" 
         << setw(25) << "Email" 
         << setw(15) << "Telepon"
         << setw(30) << "Alamat"
         << setw(12) << "Tgl Daftar" << "\n";
    cout << string(120, '=') << "\n";
    
    for (auto &m : members) {
        cout << m.toString() << endl;
    }
    cout << string(120, '=') << "\n";
    cout << "Total: " << members.size() << " anggota\n";
}

void LibrarySystem::searchMember() {
    cin.ignore();
    string key;
    cout << "\nMasukkan kata kunci (nama/email/telepon): ";
    getline(cin, key);

    if (key.empty()) {
        cout << "[ERROR] Kata kunci tidak boleh kosong!\n";
        return;
    }

    string keyLower = toLower(key);
    bool found = false;
    
    cout << "\n=== HASIL PENCARIAN ===\n";
    cout << string(120, '=') << "\n";

    for (auto &m : members) {
        string nameLower = toLower(m.getName());
        string emailLower = toLower(m.getEmail());
        string phoneLower = toLower(m.getPhone());
        
        if (nameLower.find(keyLower) != string::npos ||
            emailLower.find(keyLower) != string::npos ||
            phoneLower.find(keyLower) != string::npos) {
            
            if (!found) {
                cout << left << setw(8) << "ID" 
                     << setw(25) << "Nama" 
                     << setw(25) << "Email" 
                     << setw(15) << "Telepon"
                     << setw(30) << "Alamat"
                     << setw(12) << "Tgl Daftar" << "\n";
                cout << string(120, '=') << "\n";
            }
            cout << m.toString() << endl;
            found = true;
        }
    }

    if (!found) {
        cout << "[INFO] Tidak ada anggota yang cocok dengan kata kunci \"" << key << "\"\n";
    } else {
        cout << string(120, '=') << "\n";
    }
}

// ===========================================================
// Transaksi - FIXED
// ===========================================================

void LibrarySystem::borrowBook() {
    string bookId, memberId;
    
    cout << "\n=== PEMINJAMAN BUKU ===\n";
    cout << "ID Buku: ";
    cin >> bookId;
    
    // Jika yang login adalah member, otomatis pakai ID member yang login
    if (currentUserRole == "MEMBER") {
        memberId = currentUserId;
        cout << "ID Anggota: " << memberId << " (Anda)\n";
    } else {
        // Admin bisa input ID member siapa saja
        cout << "ID Anggota: ";
        cin >> memberId;
    }

    Book *b = nullptr;
    Member *m = nullptr;

    for (auto &bk : books) {
        if (bk.getId() == bookId) {
            b = &bk;
            break;
        }
    }
    
    for (auto &mb : members) {
        if (mb.getId() == memberId) {
            m = &mb;
            break;
        }
    }

    if (!b) {
        cout << "[ERROR] Buku tidak ditemukan\n";
        return;
    }
    if (!m) {
        cout << "[ERROR] Anggota tidak ditemukan\n";
        return;
    }

    if (b->getAvailable() <= 0) {
        cout << "[ERROR] Buku tidak tersedia untuk dipinjam\n";
        cout << "Stok: " << b->getStock() << ", Tersedia: " << b->getAvailable() << "\n";
        return;
    }

    string tid = "T";
    if (transIdCounter < 10) tid += "000";
    else if (transIdCounter < 100) tid += "00";
    else if (transIdCounter < 1000) tid += "0";
    tid += to_string(transIdCounter++);
    
    string borrow = getCurrentDate();
    string due = addDays(borrow, 14);

    transactions.push_back(Transaction(tid, bookId, memberId, borrow, due));
    b->borrowBook();

    cout << "\n[OK] Peminjaman berhasil!\n";
    cout << "==============================\n";
    cout << "ID Transaksi  : " << tid << "\n";
    cout << "Buku          : " << b->getTitle() << "\n";
    cout << "Peminjam      : " << m->getName() << "\n";
    cout << "Tgl Pinjam    : " << borrow << "\n";
    cout << "Jatuh Tempo   : " << due << " (14 hari)\n";
    cout << "==============================\n";
    cout << "\nCATAT ID Transaksi untuk pengembalian!\n";
}

void LibrarySystem::returnBook() {
    string tid;
    
    cout << "\n=== PENGEMBALIAN BUKU ===\n";
    cout << "ID Transaksi: ";
    cin >> tid;

    for (auto &t : transactions) {
        if (t.getId() == tid && t.getStatus() == "BORROWED") {
            // Jika member, cek apakah transaksi miliknya
            if (currentUserRole == "MEMBER" && t.getMemberId() != currentUserId) {
                cout << "[ERROR] Ini bukan transaksi Anda!\n";
                return;
            }
            
            string ret = getCurrentDate();
            t.processReturn(ret);

            for (auto &b : books) {
                if (b.getId() == t.getBookId()) {
                    b.returnBook();
                    break;
                }
            }

            cout << "\n[OK] Buku dikembalikan\n";
            cout << "==============================\n";
            cout << "Tanggal Kembali : " << ret << "\n";
            cout << "Jatuh Tempo     : " << t.getDueDate() << "\n";
            
            if (t.getFine() > 0) {
                int late = daysDifference(t.getDueDate(), ret);
                cout << "STATUS          : TERLAMBAT (" << late << " hari)\n";
                cout << "DENDA           : Rp " << fixed << setprecision(0) << t.getFine() << "\n";
            } else {
                cout << "STATUS          : TEPAT WAKTU\n";
                cout << "DENDA           : Rp 0\n";
            }
            cout << "==============================\n";
            return;
        }
    }

    cout << "[ERROR] Transaksi tidak ditemukan atau sudah dikembalikan\n";
}

void LibrarySystem::displayAllTransactions() {
    if (transactions.empty()) {
        cout << "\n[INFO] Tidak ada transaksi dalam sistem.\n";
        return;
    }

    cout << "\n=== RIWAYAT SEMUA TRANSAKSI ===\n";
    cout << string(120, '=') << "\n";
    cout << left << setw(10) << "ID Trans"
         << setw(10) << "ID Buku"
         << setw(10) << "ID Member"
         << setw(12) << "Tgl Pinjam"
         << setw(12) << "Jatuh Tempo"
         << setw(12) << "Tgl Kembali"
         << setw(12) << "Status"
         << "Denda" << "\n";
    cout << string(120, '=') << "\n";
    
    for (auto &t : transactions) {
        cout << t.toString() << endl;
    }
    cout << string(120, '=') << "\n";
    cout << "Total: " << transactions.size() << " transaksi\n";
}

void LibrarySystem::displayBorrowedBooks() {
    int count = 0;
    
    cout << "\n=== BUKU YANG SEDANG DIPINJAM ===\n";
    cout << string(100, '=') << "\n";

    for (auto &t : transactions) {
        if (t.getStatus() == "BORROWED") {
            if (count == 0) {
                cout << left << setw(10) << "ID Trans"
                     << setw(10) << "ID Buku"
                     << setw(10) << "ID Member"
                     << setw(12) << "Tgl Pinjam"
                     << setw(12) << "Jatuh Tempo"
                     << "Status" << "\n";
                cout << string(100, '=') << "\n";
            }
            
            cout << left << setw(10) << t.getId()
                 << setw(10) << t.getBookId()
                 << setw(10) << t.getMemberId()
                 << setw(12) << t.getBorrowDate()
                 << setw(12) << t.getDueDate();
            
            int late = daysDifference(t.getDueDate(), getCurrentDate());
            if (late > 0) {
                cout << "[!] TERLAMBAT " << late << " hari";
            } else {
                cout << "[OK]";
            }
            cout << "\n";
            count++;
        }
    }

    if (count == 0) {
        cout << "[INFO] Tidak ada buku yang sedang dipinjam.\n";
    } else {
        cout << string(100, '=') << "\n";
        cout << "Total: " << count << " buku sedang dipinjam\n";
    }
}

// Untuk member melihat bukunya sendiri
void LibrarySystem::displayMyBorrowedBooks() {
    int count = 0;
    
    cout << "\n=== BUKU YANG SAYA PINJAM ===\n";
    cout << string(100, '=') << "\n";

    for (auto &t : transactions) {
        if (t.getStatus() == "BORROWED" && t.getMemberId() == currentUserId) {
            if (count == 0) {
                cout << left << setw(10) << "ID Trans"
                     << setw(10) << "ID Buku"
                     << setw(30) << "Judul"
                     << setw(12) << "Tgl Pinjam"
                     << setw(12) << "Jatuh Tempo" << "\n";
                cout << string(100, '=') << "\n";
            }
            
            // Cari judul buku
            string judul = "";
            for (auto &b : books) {
                if (b.getId() == t.getBookId()) {
                    judul = b.getTitle();
                    break;
                }
            }
            
            cout << left << setw(10) << t.getId()
                 << setw(10) << t.getBookId()
                 << setw(30) << judul
                 << setw(12) << t.getBorrowDate()
                 << setw(12) << t.getDueDate() << "\n";
            count++;
        }
    }

    if (count == 0) {
        cout << "[INFO] Anda belum meminjam buku apapun.\n";
    } else {
        cout << string(100, '=') << "\n";
        cout << "Total: " << count << " buku\n";
    }
}

void LibrarySystem::displayStatistics() {
    int totalTitles = books.size();
    int totalCopies = 0, availCopies = 0;
    
    for (auto &b : books) {
        totalCopies += b.getStock();
        availCopies += b.getAvailable();
    }
    
    int activeBorrows = 0, completedReturns = 0;
    double totalFines = 0;
    int lateReturns = 0;
    
    for (auto &t : transactions) {
        if (t.getStatus() == "BORROWED") {
            activeBorrows++;
        } else {
            completedReturns++;
        }
        
        if (t.getFine() > 0) {
            totalFines += t.getFine();
            lateReturns++;
        }
    }

    cout << "\n========================================\n";
    cout << "    STATISTIK PERPUSTAKAAN DIGITAL\n";
    cout << "========================================\n\n";
    
    cout << "KOLEKSI BUKU:\n";
    cout << "  Total Judul           : " << totalTitles << " judul\n";
    cout << "  Total Eksemplar       : " << totalCopies << " buku\n";
    cout << "  Tersedia              : " << availCopies << " buku\n";
    cout << "  Sedang Dipinjam       : " << (totalCopies - availCopies) << " buku\n\n";
    
    cout << "KEANGGOTAAN:\n";
    cout << "  Total Anggota         : " << members.size() << " orang\n\n";
    
    cout << "TRANSAKSI:\n";
    cout << "  Total Transaksi       : " << transactions.size() << "\n";
    cout << "  Peminjaman Aktif      : " << activeBorrows << "\n";
    cout << "  Pengembalian Selesai  : " << completedReturns << "\n";
    cout << "  Keterlambatan         : " << lateReturns << " kasus\n\n";
    
    cout << "DENDA:\n";
    cout << "  Total Terkumpul       : Rp " << fixed << setprecision(0) << totalFines << "\n";
    cout << "========================================\n";
}

// ===========================================================
// MENU SYSTEM - FIXED!!!
// ===========================================================

void LibrarySystem::displayMenu() {
    while (true) {
        cout << "\n";
        cout << "==========================================================\n";
        cout << "           SISTEM PERPUSTAKAAN DIGITAL BYAN               \n";
        cout << "==========================================================\n";
        cout << "\n           SELAMAT DATANG!                               \n";
        cout << "\nSilakan pilih cara Anda ingin masuk:\n\n";
        cout << "  1. Login sebagai Admin\n";
        cout << "  2. Login sebagai Member\n";
        cout << "  0. Keluar\n";
        cout << "==========================================================\n";
        cout << "\nPilih: ";
        
        int choice;
        cin >> choice;
        
        if (choice == 1) {
            if (loginAsAdmin()) {
                displayAdminMenu();
                logout();
            }
        } else if (choice == 2) {
            if (loginAsMember()) {
                displayMemberMenu();
                logout();
            }
        } else if (choice == 0) {
            cout << "\n==========================================================\n";
            cout << "   Terima kasih telah menggunakan Sistem Perpustakaan!   \n";
            cout << "==========================================================\n";
            break;
        } else {
            cout << "\n[ERROR] Pilihan tidak valid!\n";
        }
    }
}

void LibrarySystem::displayAdminMenu() {
    int choice;

    do {
        cout << "\n";
        cout << "==========================================================\n";
        cout << "       SISTEM MANAJEMEN PERPUSTAKAAN DIGITAL              \n";
        cout << "                  MODE: ADMINISTRATOR                     \n";
        cout << "==========================================================\n";
        cout << "\n--- MANAJEMEN BUKU ---\n";
        cout << "  1. Tambah Buku\n";
        cout << "  2. Tampilkan Semua Buku\n";
        cout << "  3. Cari Buku\n";
        cout << "  4. Edit Buku\n";
        cout << "  5. Hapus Buku\n";
        cout << "\n--- MANAJEMEN ANGGOTA ---\n";
        cout << "  6. Tambah Anggota\n";
        cout << "  7. Tampilkan Semua Anggota\n";
        cout << "  8. Cari Anggota\n";
        cout << "  9. Edit Anggota\n";
        cout << " 10. Hapus Anggota\n";
        cout << "\n--- TRANSAKSI ---\n";
        cout << " 11. Pinjam Buku\n";
        cout << " 12. Kembalikan Buku\n";
        cout << " 13. Tampilkan Semua Transaksi\n";
        cout << " 14. Tampilkan Buku yang Sedang Dipinjam\n";
        cout << "\n--- LAINNYA ---\n";
        cout << " 15. Statistik Perpustakaan\n";
        cout << " 16. Simpan Data ke File\n";
        cout << " 17. Muat Data dari File\n";
        cout << "  0. Logout\n";
        cout << "==========================================================\n";
        cout << "\nPilih menu: ";
        cin >> choice;

        switch (choice) {
            case 1: addBook(); break;
            case 2: displayAllBooks(); break;
            case 3: searchBook(); break;
            case 4: editBook(); break;
            case 5: deleteBook(); break;
            case 6: addMember(); break;
            case 7: displayAllMembers(); break;
            case 8: searchMember(); break;
            case 9: editMember(); break;
            case 10: deleteMember(); break;
            case 11: borrowBook(); break;
            case 12: returnBook(); break;
            case 13: displayAllTransactions(); break;
            case 14: displayBorrowedBooks(); break;
            case 15: displayStatistics(); break;
            case 16: saveToFile("output.txt"); break;
            case 17: loadFromFile("input.txt"); break;
            case 0:
                cout << "\n[INFO] Logout dari Admin...\n";
                break;
            default:
                cout << "\n[ERROR] Pilihan tidak valid!\n";
        }

        if (choice != 0) {
            cout << "\nTekan Enter untuk melanjutkan...";
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cin.get();
        }

    } while (choice != 0);
}

void LibrarySystem::displayMemberMenu() {
    int choice;

    do {
        cout << "\n";
        cout << "==========================================================\n";
        cout << "       SISTEM PERPUSTAKAAN DIGITAL - MODE MEMBER         \n";
        cout << "       User: " << currentUserId;
        
        // Tampilkan nama member
        for (auto &m : members) {
            if (m.getId() == currentUserId) {
                cout << " (" << m.getName() << ")";
                break;
            }
        }
        cout << "\n";
        cout << "==========================================================\n";
        cout << "\n--- KATALOG BUKU ---\n";
        cout << "  1. Tampilkan Semua Buku\n";
        cout << "  2. Cari Buku\n";
        cout << "\n--- TRANSAKSI SAYA ---\n";
        cout << "  3. Pinjam Buku\n";
        cout << "  4. Kembalikan Buku\n";
        cout << "  5. Lihat Buku yang Saya Pinjam\n";
        cout << "\n--- LAINNYA ---\n";
        cout << "  0. Logout\n";
        cout << "==========================================================\n";
        cout << "\nPilih menu: ";
        cin >> choice;

        switch (choice) {
            case 1: 
                displayAllBooks(); 
                break;
                
            case 2: 
                searchBook(); 
                break;
                
            case 3: 
                borrowBook(); 
                break;
                
            case 4: 
                returnBook(); 
                break;
                
            case 5: 
                displayMyBorrowedBooks(); 
                break;
                
            case 0:
                cout << "\n[INFO] Logout dari Member...\n";
                break;
                
            default:
                cout << "\n[ERROR] Pilihan tidak valid!\n";
        }

        if (choice != 0) {
            cout << "\nTekan Enter untuk melanjutkan...";
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cin.get();
        }

    } while (choice != 0);
}