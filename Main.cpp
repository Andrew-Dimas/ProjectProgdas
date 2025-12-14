#include <iostream>
#include <fstream>
#include <windows.h>
#include <io.h>
#include <fcntl.h>
#include "LibrarySystem.h"
using namespace std;

int main(int argc, char**argv){
    string in="input.txt", out="output.txt";
    
    for(int i=1; i<argc; i++){
        string a=argv[i];
        if(a.rfind("--input=",0)==0) in=a.substr(8);
        if(a.rfind("--output=",0)==0) out=a.substr(9);
    }

    LibrarySystem sys;
    sys.loadFromFile(in);
    sys.displayMenu();
    sys.saveToFile(out);
    
    // Copy output.txt ke input.txt
    cout << "\n[INFO] Menyinkronkan data...\n";
    
    // Baca output.txt
    ifstream src(out, ios::binary);
    if (!src.is_open()) {
        cout << "[ERROR] Gagal membaca " << out << "\n";
        return 1;
    }
    
    // Tulis ke input.txt
    ofstream dst(in, ios::binary);
    if (!dst.is_open()) {
        cout << "[ERROR] Gagal menulis ke " << in << "\n";
        return 1;
    }
    
    // Copy content
    dst << src.rdbuf();
    
    src.close();
    dst.close();
    
    cout << "[OK] Data berhasil disinkronkan dari " << out << " ke " << in << "\n";
    cout << "Saat program dijalankan lagi, data terbaru akan dimuat.\n";
    
    return 0;
}