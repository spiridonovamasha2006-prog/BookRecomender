#include "CustomProduct.h"
#include <cstring>
#include <stdexcept>

CustomProduct::CustomProduct(int id, const std::string& name, double price,
    const std::string& category, const std::string& customData)
    : Product(id, name, "Custom product", price, category) {
    if (customData.empty())
        throw std::invalid_argument("customData must not be empty");
    customData_ = new char[customData.size() + 1];
    std::strcpy(customData_, customData.c_str());
}

CustomProduct::~CustomProduct() {
    delete[] customData_;
}

const std::string CustomProduct::getCustomData() const {
    return customData_ ? std::string(customData_) : "";
}

std::unique_ptr<Product> CustomProduct::clone() const {
    return std::make_unique<CustomProduct>(id_, name_, price_, category_, getCustomData());
}