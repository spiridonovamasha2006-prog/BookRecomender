#pragma once
#include <memory>
#include <ctime>
class User;
class Product;

class Rating {
public:
    Rating();
    Rating(int id, std::shared_ptr<User> user, std::shared_ptr<Product> product,
        int score, std::time_t timestamp = std::time(nullptr));
    Rating(const Rating&) = default;
    Rating& operator=(const Rating&) = default;
    ~Rating() = default;

    int getId() const;
    std::shared_ptr<User> getUser() const;
    std::shared_ptr<Product> getProduct() const;
    int getScore() const;
    std::time_t getTimestamp() const;
    explicit operator int() const;

private:
    int id_;
    std::shared_ptr<User> user_;
    std::shared_ptr<Product> product_;
    int score_;
    std::time_t timestamp_;
};