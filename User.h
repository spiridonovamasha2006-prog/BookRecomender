#pragma once
#include <string>
#include <vector>
#include <memory>
class Purchase;
class Cart;
class Product;

class User : public std::enable_shared_from_this<User> {
public:
    User();
    User(int id, const std::string& name, const std::string& email, int age, const std::string& passwordHash);
    User(const User&) = delete;
    User& operator=(const User&) = delete;
    ~User() = default;

    int getId() const;
    const std::string& getName() const;
    const std::string& getEmail() const;
    int getAge() const;
    const std::string& getPasswordHash() const;
    std::vector<std::shared_ptr<Purchase>>& getPurchaseHistory();
    void addToCart(const std::shared_ptr<Product>& product);
    std::shared_ptr<Cart> getCart() const;
    void rateProduct(const std::shared_ptr<Product>& product, int rating);

private:
    int id_;
    std::string name_;
    std::string email_;
    int age_;
    std::string passwordHash_;
    std::vector<std::shared_ptr<Purchase>> purchaseHistory_;
    std::unique_ptr<Cart> cart_;
};