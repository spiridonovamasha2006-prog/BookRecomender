#pragma once
#include <vector>
#include <memory>
#include <algorithm>
#include <functional>  // ← ДОБАВИТЬ ЭТУ СТРОКУ для std::function
#include <type_traits>
#include "IPurchasable.h"

template<typename T>
class Catalog {
    static_assert(std::is_base_of_v<IPurchasable, T>, "T must be derived from IPurchasable");

public:
    void add(std::shared_ptr<T> item) {
        items_.push_back(item);
    }

    void sortByPrice() {
        std::sort(items_.begin(), items_.end(),
            [](const std::shared_ptr<T>& a, const std::shared_ptr<T>& b) {
                return a->getPrice() < b->getPrice();
            });
    }

    std::vector<std::shared_ptr<T>> findIf(std::function<bool(const std::shared_ptr<T>&)> predicate) {
        std::vector<std::shared_ptr<T>> result;
        for (const auto& item : items_) {
            if (predicate(item)) {
                result.push_back(item);
            }
        }
        return result;
    }

    template<typename TransformFunc>
    auto transform(TransformFunc func) const {
        std::vector<decltype(func(std::declval<std::shared_ptr<T>>()))> result;
        for (const auto& item : items_) {
            result.push_back(func(item));
        }
        return result;
    }

    double getTotalPrice() const {
        double total = 0.0;
        for (const auto& item : items_) {
            total += item->getPrice();
        }
        return total;
    }

    const std::vector<std::shared_ptr<T>>& getAll() const {
        return items_;
    }

    size_t size() const {
        return items_.size();
    }

    bool empty() const {
        return items_.empty();
    }

private:
    std::vector<std::shared_ptr<T>> items_;
};