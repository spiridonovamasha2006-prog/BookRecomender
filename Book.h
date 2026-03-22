#pragma once
#include "Product.h"
#include <string>

class Book : public Product {
public:
    Book(int id, const std::string& title, const std::string& author,
        double price, const std::string& isbn);

    const std::string& getAuthor() const;
    const std::string& getIsbn() const;

private:
    std::string author_;
    std::string isbn_;
};