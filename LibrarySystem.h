#pragma once
#include <vector>
#include <string>
#include "Book.h"
#include "Member.h"
#include "Transaction.h"
using namespace std;

class LibrarySystem {
private:
    vector<Book> books;
    vector<Member> members;
    vector<Transaction> transactions;
    int bookIdCounter = 1;
    int memberIdCounter = 1;
    int transIdCounter = 1;
    
    // AUTHENTICATION - BARU!
    string currentUserRole = "";  // "ADMIN" atau "MEMBER"
    string currentUserId = "";    // ID member atau "ADMIN"

public:
    // File I/O
    void loadFromFile(const string& path);
    void saveToFile(const string& path);

    // Authentication
    bool loginAsAdmin();
    bool loginAsMember();
    void logout();

    // Book Management
    void addBook();
    void editBook();
    void deleteBook();
    void displayAllBooks();
    void searchBook();

    // Member Management
    void addMember();
    void editMember();
    void deleteMember();
    void displayAllMembers();
    void searchMember();

    // Transaction
    void borrowBook();
    void returnBook();
    void displayAllTransactions();
    void displayBorrowedBooks();
    void displayMyBorrowedBooks();

    // Statistics
    void displayStatistics();

    // Menu
    void displayMenu();
    void displayAdminMenu();
    void displayMemberMenu();
};