#include "Purchase.h"
#include "User.h"
#include "Product.h"
#include <stdexcept>

int& Purchase::getNextIdRef() {
    static int nextId = 1;
    return nextId;
}

Purchase::Purchase(std::shared_ptr<User> user, std::time_t purchaseDate)
    : id_(getNextIdRef()++), user_(std::move(user)), purchaseDate_(purchaseDate), totalAmount_(0.0) {
    if (!user_) {
        throw std::invalid_argument("user must not be null");
    }
}

int Purchase::getId() const { return id_; }
std::shared_ptr<User> Purchase::getUser() const { return user_; }
const std::vector<std::pair<std::shared_ptr<Product>, int>>& Purchase::getProducts() const {
    return products_;
}

std::time_t Purchase::getPurchaseDate() const { return purchaseDate_; }
double Purchase::getTotalAmount() const { return totalAmount_; }

void Purchase::addProduct(const std::shared_ptr<Product>& product, int quantity) {
    if (!product) {
        throw std::invalid_argument("product must not be null");
    }
    if (quantity <= 0) {
        throw std::invalid_argument("quantity must be > 0");
    }
    products_.emplace_back(product, quantity);
    totalAmount_ += product->getPrice() * quantity;
}