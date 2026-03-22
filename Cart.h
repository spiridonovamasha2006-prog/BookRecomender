#pragma once
#include <vector>
#include <memory>
#include <string>

class User;
class Product;
class Purchase;

class Cart {
public:
    explicit Cart(std::weak_ptr<User> user);

    Cart(const Cart&) = delete;
    Cart& operator=(const Cart&) = delete;
    Cart(Cart&&) noexcept = default;
    Cart& operator=(Cart&&) noexcept = default;
    ~Cart() = default;

    int getId() const;
    std::weak_ptr<User> getUser() const;
    const std::vector<std::pair<std::shared_ptr<Product>, int>>& getItems() const;

    void addItem(const std::shared_ptr<Product>& product, int quantity = 1);
    void removeItem(const std::shared_ptr<Product>& product);
    double getTotalPrice() const;
    std::unique_ptr<Purchase> checkout();

    explicit operator bool() const;

private:
    static int& getNextIdRef() {
        static int nextId = 1;
        return nextId;
    }

    int id_;
    std::weak_ptr<User> user_;
    std::vector<std::pair<std::shared_ptr<Product>, int>> items_;
};