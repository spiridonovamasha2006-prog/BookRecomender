#include "User.h"
#include "Cart.h"
#include "Purchase.h"
#include "Product.h"
#include <stdexcept>

User::User()
    : id_(0), age_(0) {
}

User::User(int id, const std::string& name, const std::string& email, int age, const std::string& passwordHash)
    : id_(id), name_(name), email_(email), age_(age), passwordHash_(passwordHash) {
    cart_ = std::make_unique<Cart>(shared_from_this());
}

int User::getId() const { return id_; }
const std::string& User::getName() const { return name_; }
const std::string& User::getEmail() const { return email_; }
int User::getAge() const { return age_; }
const std::string& User::getPasswordHash() const { return passwordHash_; }

std::vector<std::shared_ptr<Purchase>>& User::getPurchaseHistory() {
    return purchaseHistory_;
}

void User::addToCart(const std::shared_ptr<Product>& product) {
    if (!cart_) {
        throw std::logic_error("Cart is null");
    }
    cart_->addItem(product);
}

std::shared_ptr<Cart> User::getCart() const {
    return cart_ ? std::shared_ptr<Cart>(const_cast<Cart*>(cart_.get()), [](Cart*) {}) : nullptr;
}

void User::rateProduct(const std::shared_ptr<Product>& product, int rating) {
    if (!product) {
        throw std::invalid_argument("product must not be null");
    }
    if (rating < 1 || rating > 5) {
        throw std::invalid_argument("rating must be 1–5");
    }
}