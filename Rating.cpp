#include "Rating.h"
#include "User.h"
#include "Product.h"
#include <stdexcept>

Rating::Rating() : id_(0), score_(0), timestamp_(0) {}

Rating::Rating(int id, std::shared_ptr<User> user, std::shared_ptr<Product> product,
    int score, std::time_t timestamp)
    : id_(id), user_(std::move(user)), product_(std::move(product)), score_(score), timestamp_(timestamp) {
    if (!user || !product) {
        throw std::invalid_argument("user and product must not be null");
    }
    if (score < 1 || score > 5) {
        throw std::invalid_argument("score must be 1–5");
    }
}

int Rating::getId() const { return id_; }
std::shared_ptr<User> Rating::getUser() const { return user_; }
std::shared_ptr<Product> Rating::getProduct() const { return product_; }
int Rating::getScore() const { return score_; }
std::time_t Rating::getTimestamp() const { return timestamp_; }
Rating::operator int() const { return score_; }