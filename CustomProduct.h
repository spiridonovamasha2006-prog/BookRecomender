#pragma once
#include "Product.h"
#include <string>

class CustomProduct : public Product {
public:
    CustomProduct(int id, const std::string& name, double price,
        const std::string& category, const std::string& customData);

    ~CustomProduct();

    const std::string getCustomData() const;

    std::unique_ptr<Product> clone() const override;

    CustomProduct(const CustomProduct&) = delete;
    CustomProduct& operator=(const CustomProduct&) = delete;

private:
    char* customData_;
};