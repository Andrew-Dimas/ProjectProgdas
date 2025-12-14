#include "Book.h"
#include <sstream>
#include <iomanip>

string Book::toString() const {
    stringstream ss;
    ss << left << setw(8) << id
       << setw(30) << title
       << setw(20) << author
       << setw(15) << isbn
       << setw(15) << category
       << setw(6) << stock
       << setw(6) << available;
    return ss.str();
}

string Book::toFile() const {
    return id + "|" + title + "|" + author + "|" + isbn + "|" + category + "|" +
           to_string(stock) + "|" + to_string(available);
}

Book Book::fromFile(const string& line) {
    stringstream ss(line);
    string id, title, author, isbn, category, stockStr, availStr;
    getline(ss, id, '|');
    getline(ss, title, '|');
    getline(ss, author, '|');
    getline(ss, isbn, '|');
    getline(ss, category, '|');
    getline(ss, stockStr, '|');
    getline(ss, availStr, '|');
    Book b(id, title, author, isbn, category, stoi(stockStr));
    b.setAvailable(stoi(availStr));
    return b;
}