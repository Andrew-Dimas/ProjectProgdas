#pragma once
#include <string>
using namespace std;

class Member {
private:
    string id, name, email, phone, address, joinDate;

public:
    Member() {}
    Member(string id, string name, string email, string phone,
           string address, string joinDate)
        : id(id), name(name), email(email), phone(phone),
          address(address), joinDate(joinDate) {}

    // Existing getters
    string getId() const { return id; }
    string getName() const { return name; }
    
    string getEmail() const { return email; }
    string getPhone() const { return phone; }
    string getAddress() const { return address; }
    string getJoinDate() const { return joinDate; }

    void setName(string v) { name = v; }
    void setEmail(string v) { email = v; }
    void setPhone(string v) { phone = v; }
    void setAddress(string v) { address = v; }

    string toFile() const;
    static Member fromFile(const string& line);
    string toString() const;
};