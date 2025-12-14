#pragma once
#include <string>
using namespace std;

class Transaction {
private:
    string id, bookId, memberId, borrowDate, dueDate, returnDate, status;
    double fine;

public:
    Transaction() : fine(0) {}
    Transaction(string id, string b, string m, string br, string du)
        : id(id), bookId(b), memberId(m), borrowDate(br),
          dueDate(du), returnDate(""), status("BORROWED"), fine(0) {}

    string getId() const { return id; }
    string getBookId() const { return bookId; }
    string getMemberId() const { return memberId; }
    
    // getters for dates and status
    string getBorrowDate() const { return borrowDate; }
    string getDueDate() const { return dueDate; }
    string getReturnDate() const { return returnDate; }
    string getStatus() const { return status; }
    double getFine() const { return fine; }

    void processReturn(string retDate);

    string toFile() const;
    static Transaction fromFile(const string& line);
    string toString() const;
};