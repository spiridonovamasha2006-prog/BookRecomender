#pragma once
#include <vector>
#include <memory>
#include <ctime>

class User;
class Product;

class Purchase {
public:
    explicit Purchase(std::shared_ptr<User> user, std::time_t purchaseDate = std::time(nullptr));

    Purchase(const Purchase&) = delete;
    Purchase& operator=(const Purchase&) = delete;
    Purchase(Purchase&&) noexcept = default;
    Purchase& operator=(Purchase&&) noexcept = default;
    ~Purchase() = default;

    int getId() const;
    std::shared_ptr<User> getUser() const;
    const std::vector<std::pair<std::shared_ptr<Product>, int>>& getProducts() const;
    std::time_t getPurchaseDate() const;
    double getTotalAmount() const;

    void addProduct(const std::shared_ptr<Product>& product, int quantity);

private:
    static int& getNextIdRef() {
        static int nextId = 1;
        return nextId;
    }

    int id_;
    std::shared_ptr<User> user_;
    std::vector<std::pair<std::shared_ptr<Product>, int>> products_;
    std::time_t purchaseDate_;
    double totalAmount_;
};