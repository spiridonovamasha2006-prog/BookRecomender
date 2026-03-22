#include "IPurchasable.h"
#include <stdexcept>

double IPurchasable::applyDiscount(double discountPercent) const {
    if (discountPercent < 0 || discountPercent > 100)
        throw std::invalid_argument("discountPercent must be in [0, 100]");
    return getPrice() * (1.0 - discountPercent / 100.0);
}