#include "Transaction.h"
#include <sstream>
#include <iomanip>

// DEKLARASI fungsi dari LibrarySystem.cpp
extern int daysDifference(const string &date1, const string &date2);


void Transaction::processReturn(string retDate) {
    returnDate = retDate;
    status = "RETURNED";
    int late = daysDifference(dueDate, retDate);
    if (late > 0) fine = late * 1000;
}

string Transaction::toFile() const {
    return id + "|" + bookId + "|" + memberId + "|" + borrowDate + "|" +
           dueDate + "|" + returnDate + "|" + to_string(fine) + "|" + status;
}

Transaction Transaction::fromFile(const string& line) {
    stringstream ss(line);
    string id, b, m, br, du, ret, fi, st;
    getline(ss, id, '|');
    getline(ss, b, '|');
    getline(ss, m, '|');
    getline(ss, br, '|');
    getline(ss, du, '|');
    getline(ss, ret, '|');
    getline(ss, fi, '|');
    getline(ss, st, '|');
    Transaction t(id, b, m, br, du);
    t.returnDate = ret;
    t.fine = stod(fi);
    t.status = st;
    return t;
}

string Transaction::toString() const {
    stringstream ss;
    ss << left << setw(10) << id 
       << setw(10) << bookId 
       << setw(10) << memberId
       << setw(12) << borrowDate 
       << setw(12) << dueDate
       << setw(12) << (returnDate == "" ? "-" : returnDate)
       << setw(10) << status 
       << "Rp " << fixed << setprecision(0) << fine;
    return ss.str();
}