#pragma once
#include <vector>
#include <memory>
#include <algorithm>
#include <numeric>
#include <type_traits>
#include "IPurchasable.h"

template <typename T>
class Catalog {
    static_assert(std::is_base_of_v<IPurchasable, T>,
        "T must inherit from IPurchasable");

public:
    Catalog() = default;

    void add(const std::shared_ptr<T>& item) {
        if (!item) throw std::invalid_argument("item must not be null");
        items_.push_back(item);
    }

    size_t size() const { return items_.size(); }

    // Не шаблонный метод: сортировка по цене
    void sortByPrice(bool ascending = true) {
        std::sort(items_.begin(), items_.end(),
            [ascending](const auto& a, const auto& b) {
                return ascending ? (a->getPrice() < b->getPrice())
                    : (a->getPrice() > b->getPrice());
            });
    }

    // Не шаблонный метод: суммарная цена
    double getTotalPrice() const {
        return std::accumulate(items_.begin(), items_.end(), 0.0,
            [](double sum, const auto& p) { return sum + p->getPrice(); });
    }

    // Шаблонный метод: поиск по предикату
    template <typename Predicate>
    std::vector<std::shared_ptr<T>> findIf(Predicate pred) const {
        std::vector<std::shared_ptr<T>> result;
        std::copy_if(items_.begin(), items_.end(),
            std::back_inserter(result), pred);
        return result;
    }

    // Шаблонный метод: трансформация (например, в имена)
    template <typename Transform>
    auto transform(Transform f) const {
        std::vector<std::invoke_result_t<Transform, const std::shared_ptr<T>&>> res;
        res.reserve(items_.size());
        for (const auto& item : items_) {
            res.push_back(f(item));
        }
        return res;
    }

    // Не шаблонный метод: вывод
    void printAll() const {
        for (const auto& item : items_) {
            std::cout << "  " << item->toDisplayString() << "\n";
        }
    }

    // Вспомогательный метод: readonly-вид (аналог span)
    auto view() const { return std::span<const std::shared_ptr<T>>(items_); }

private:
    std::vector<std::shared_ptr<T>> items_;
};