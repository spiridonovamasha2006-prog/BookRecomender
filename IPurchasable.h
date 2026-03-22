#pragma once
#include <string>

class IPurchasable {
public:
    virtual ~IPurchasable() = default;
    virtual double getPrice() const = 0;
    virtual std::string toDisplayString() const = 0;
    double applyDiscount(double discountPercent) const;
};