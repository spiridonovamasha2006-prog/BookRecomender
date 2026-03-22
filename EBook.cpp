#include "EBook.h"
#include <sstream>
#include <iomanip>

EBook::EBook(int id, const std::string& title, const std::string& author,
    double price, const std::string& isbn, const std::string& format)
    : Product(id, title, "", price, "Электронная книга"),
    author_(author), isbn_(isbn), format_(format) {
    addTag("ebook");
    addTag("digital");
}

const std::string& EBook::getAuthor() const { return author_; }
const std::string& EBook::getIsbn() const { return isbn_; }
const std::string& EBook::getFormat() const { return format_; }

std::string EBook::toDisplayString() const {
    std::ostringstream oss;
    oss << "«" << name_ << "» (" << format_ << ", " << category_ << ") — "
        << std::fixed << std::setprecision(2) << getPrice() << "₽ (электронная)";
    return oss.str();
}

double EBook::getPrice() const {
    double basePrice = Product::getPrice();
    return std::max(50.0, basePrice * 0.9);
}

std::unique_ptr<Product> EBook::clone() const {
    return std::make_unique<EBook>(*this);
}

EBook& EBook::operator=(const Product& other) {
    if (this == &other) return *this;

    id_ = other.id_;
    name_ = other.name_;
    price_ = other.price_;
    category_ = other.category_;
    description_ = other.description_;
    tags_ = other.tags_;

    return *this;
}