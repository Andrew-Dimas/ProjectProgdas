#pragma once
#include <string>
using namespace std;

class Book {
private:
    string id, title, author, isbn, category;
    int stock, available;

public:
    Book() : stock(0), available(0) {}
    Book(string id, string title, string author, string isbn, string category, int stock)
        : id(id), title(title), author(author), isbn(isbn), category(category),
          stock(stock), available(stock) {}

    string getId() const { return id; }
    string getTitle() const { return title; }
    string getAuthor() const { return author; }
    string getIsbn() const { return isbn; }
    string getCategory() const { return category; }
    int getStock() const { return stock; }
    int getAvailable() const { return available; }

    void setTitle(string v) { title = v; }
    void setAuthor(string v) { author = v; }
    void setIsbn(string v) { isbn = v; }
    void setCategory(string v) { category = v; }
    void setStock(int v) { stock = v; }
    void setAvailable(int v) { available = v; }

    void borrowBook() { if (available > 0) available--; }
    void returnBook() { if (available < stock) available++; }

    string toFile() const;
    static Book fromFile(const string& line);
    string toString() const;
};
