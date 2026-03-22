#include "Cart.h"
#include "User.h"
#include "Product.h"
#include "Purchase.h"
#include <stdexcept>
#include <algorithm>

int& Cart::getNextIdRef() {
    static int nextId = 1;
    return nextId;
}

Cart::Cart(std::weak_ptr<User> user)
    : user_(std::move(user)), id_(getNextIdRef()++) {
    if (user.expired()) {
        throw std::invalid_argument("user must not be null");
    }
}

int Cart::getId() const { return id_; }
std::weak_ptr<User> Cart::getUser() const { return user_; }
const std::vector<std::pair<std::shared_ptr<Product>, int>>& Cart::getItems() const { return items_; }

void Cart::addItem(const std::shared_ptr<Product>& product, int quantity) {
    if (!product) throw std::invalid_argument("product must not be null");
    if (quantity <= 0) throw std::invalid_argument("quantity must be > 0");
    auto it = std::find_if(items_.begin(), items_.end(),
        [&product](const auto& item) { return item.first == product; });
    if (it != items_.end()) {
        it->second += quantity;
    }
    else {
        items_.emplace_back(product, quantity);
    }
}

void Cart::removeItem(const std::shared_ptr<Product>& product) {
    if (!product) return;
    items_.erase(std::remove_if(items_.begin(), items_.end(),
        [&product](const auto& item) { return item.first == product; }),
        items_.end());
}

double Cart::getTotalPrice() const {
    double total = 0.0;
    for (const auto& item : items_) {
        total += item.first->getPrice() * item.second;
    }
    return total;
}

std::unique_ptr<Purchase> Cart::checkout() {
    if (items_.empty()) {
        throw std::runtime_error("Cannot checkout empty cart");
    }
    auto lockedUser = user_.lock();
    if (!lockedUser) {
        throw std::runtime_error("User no longer exists");
    }
    auto purchase = std::make_unique<Purchase>(lockedUser);
    for (const auto& item : items_) {
        purchase->addProduct(item.first, item.second);
    }
    items_.clear();
    return purchase;
}

Cart::operator bool() const { return !items_.empty(); }