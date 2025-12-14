#include "Member.h"
#include <sstream>
#include <iomanip>

string Member::toString() const {
    stringstream ss;
    ss << left << setw(8) << id
       << setw(25) << name
       << setw(25) << email
       << setw(15) << phone
       << setw(30) << address
       << setw(12) << joinDate;
    return ss.str();
}

string Member::toFile() const {
    return id + "|" + name + "|" + email + "|" + phone + "|" + address + "|" + joinDate;
}

Member Member::fromFile(const string& line) {
    stringstream ss(line);
    string id, name, email, phone, address, joinDate;
    getline(ss, id, '|');
    getline(ss, name, '|');
    getline(ss, email, '|');
    getline(ss, phone, '|');
    getline(ss, address, '|');
    getline(ss, joinDate, '|');
    return Member(id, name, email, phone, address, joinDate);
}