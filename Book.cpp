#include "Book.h"

Book::Book(int id, const std::string& title, const std::string& author,
    double price, const std::string& isbn)
    : Product(id, title, "", price, "Книга"), author_(author), isbn_(isbn) {
    addTag("book");
    addTag("paper");
}

const std::string& Book::getAuthor() const { return author_; }
const std::string& Book::getIsbn() const { return isbn_; }