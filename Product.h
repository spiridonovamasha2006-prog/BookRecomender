#pragma once
#include <string>
#include <vector>
#include <memory>
#include <ostream>
#include "IPurchasable.h"

class Product : public IPurchasable {
public:
    Product();
    Product(int id, const std::string& name, const std::string& description,
        double price, const std::string& category);
    Product(const Product&) = default;
    Product& operator=(const Product&) = default;
    virtual ~Product() = default;

    double getPrice() const override;
    std::string toDisplayString() const override;
    int getId() const;
    const std::string& getName() const;
    const std::string& getDescription() const;
    const std::string& getCategory() const;
    const std::vector<std::string>& getTags() const;

    Product& setName(const std::string& name);
    Product& setDescription(const std::string& desc);
    Product& setPrice(double price);
    Product& setCategory(const std::string& cat);
    void addTag(const std::string& tag);
    bool containsTag(const std::string& tag) const;

    virtual std::unique_ptr<Product> clone() const = 0;
    bool operator==(const Product& other) const;
    bool operator!=(const Product& other) const;
    bool operator<(const Product& other) const;
    friend std::ostream& operator<<(std::ostream& os, const Product& p);

protected:
    int id_;
    std::string name_;
    std::string description_;
    double price_;
    std::string category_;
    std::vector<std::string> tags_;

private:
    static int& getNextIdRef();
};