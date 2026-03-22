#pragma once
#include "Product.h"
#include <string>

class EBook : public Product {
public:
    EBook(int id, const std::string& title, const std::string& author,
        double price, const std::string& isbn, const std::string& format = "PDF");
    const std::string& getAuthor() const;
    const std::string& getIsbn() const;
    const std::string& getFormat() const;
    std::string toDisplayString() const override;
    double getPrice() const override;
    std::unique_ptr<Product> clone() const override;
private:
    std::string author_;
    std::string isbn_;
    std::string format_;
};