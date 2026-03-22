#include <iostream>
#include <vector>
#include <string>
#include <memory>
#include <stdexcept>
#include <algorithm>
#include <numeric>
#include <ranges>
#include <span>
#include <type_traits>

// Ваши заголовки (из прога6.docx)
#include "IPurchasable.h"
#include "Product.h"
#include "Book.h"
#include "EBook.h"
#include "CustomProduct.h"
#include "User.h"
#include "Cart.h"
#include "Purchase.h"
#include "Rating.h"

template <typename T>
concept HasGetPrice = requires(const T & x) {
    { x.getPrice() } -> std::convertible_to<double>;
};

// Шаблонная функция: средняя цена по коллекции указателей
template <typename Range>
    requires std::ranges::input_range<Range>&&
HasGetPrice<std::remove_cvref_t<
    std::remove_pointer_t<
    std::remove_cvref_t<
    std::remove_reference_t<decltype(*std::declval<Range>().begin())>>>>>
    double averagePrice(const Range& container) {
    if (container.empty()) return 0.0;
    double sum = 0.0, count = 0.0;
    for (const auto& ptr : container) {
        if (ptr) {
            sum += ptr->getPrice();
            ++count;
        }
    }
    return count == 0 ? 0.0 : sum / count;
}

// Имитация Catalog<T> прямо в main.cpp (можно вынести в Catalog.h — рекомендуется)
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

    void sortByPrice(bool ascending = true) {
        std::sort(items_.begin(), items_.end(),
            [ascending](const auto& a, const auto& b) {
                return ascending ? (a->getPrice() < b->getPrice())
                    : (a->getPrice() > b->getPrice());
            });
    }

    double getTotalPrice() const {
        return std::accumulate(items_.begin(), items_.end(), 0.0,
            [](double sum, const auto& p) { return sum + p->getPrice(); });
    }

    template <typename Predicate>
    std::vector<std::shared_ptr<T>> findIf(Predicate pred) const {
        std::vector<std::shared_ptr<T>> result;
        std::copy_if(items_.begin(), items_.end(),
            std::back_inserter(result), pred);
        return result;
    }

    template <typename Transform>
    auto transform(Transform f) const {
        std::vector<std::invoke_result_t<Transform, const std::shared_ptr<T>&>> res;
        res.reserve(items_.size());
        for (const auto& item : items_) {
            res.push_back(f(item));
        }
        return res;
    }

    void printAll() const {
        for (const auto& item : items_) {
            std::cout << "  " << item->toDisplayString() << "\n";
        }
    }

private:
    std::vector<std::shared_ptr<T>> items_;
};

// ========== Вспомогательные функции (как в прога6.docx) ==========
void demoVirtualFunctions() {
    std::cout << "\n--- Демонстрация виртуальных функций ---\n";
    std::vector<std::shared_ptr<Product>> products;
    products.push_back(std::make_shared<Book>(1, "1984", "Orwell", 800.0, "978-0-452-28423-4"));
    products.push_back(std::make_shared<EBook>(2, "Dune", "Herbert", 600.0, "978-0-441-17271-9", "EPUB"));
    products.push_back(std::make_shared<CustomProduct>(3, "Sticker Pack", 150.0, "Stationery", "Funny Cats"));

    for (const auto& p : products) {
        std::cout << p->toDisplayString() << "\n";
    }
}

void demoCloning() {
    std::cout << "\n--- Демонстрация клонирования ---\n";
    auto book = std::make_shared<Book>(1, "1984", "Orwell", 800.0, "978-0-452-28423-4");
    auto clone = book->clone();
    std::cout << "Оригинал: " << book->toDisplayString() << "\n";
    std::cout << "Клон:     " << clone->toDisplayString() << "\n";
}

// ========== Основная функция ==========
int main() {
    std::cout << "=== Интернет-магазин (Лабораторные №6 + №8) ===\n";

    // Создаём товары
    std::vector<std::shared_ptr<Product>> products;
    products.push_back(std::make_shared<Book>(1, "1984", "Orwell", 800.0, "978-0-452-28423-4"));
    products.push_back(std::make_shared<EBook>(2, "Dune", "Herbert", 600.0, "978-0-441-17271-9", "EPUB"));
    products.push_back(std::make_shared<CustomProduct>(3, "Sticker Pack", 150.0, "Stationery", "Funny Cats"));
    products.push_back(std::make_shared<Book>(4, "Fahrenheit 451", "Bradbury", 720.0, "978-1-4516-7331-9"));
    products.push_back(std::make_shared<EBook>(5, "Neuromancer", "Gibson", 550.0, "978-0-441-56959-5", "PDF"));

    std::shared_ptr<User> user;
    std::shared_ptr<Cart> cart;

    int choice;
    do {
        std::cout << "\nМеню:\n";
        std::cout << "1. Показать все товары\n";
        std::cout << "2. Добавить товар в корзину\n";
        std::cout << "3. Показать корзину\n";
        std::cout << "4. Оформить заказ\n";
        std::cout << "5. Добавить пользователя\n";
        std::cout << "6. Поставить оценку\n";
        std::cout << "7. Показать товары с оценкой\n";
        std::cout << "8. Поиск товара по ID\n";
        std::cout << "9. Показать данные (все структуры)\n";
        std::cout << "10. Демо: виртуальные функции\n";
        std::cout << "11. Демо: клонирование\n";
        std::cout << "12. Сортировка товаров по цене\n";  
        std::cout << "13. Найти первый eBook\n";        
        std::cout << "14. Средняя цена всех товаров\n";  
        std::cout << "15. Демо: шаблонный Catalog<Product>\n"; 
        std::cout << "0. Выход\n";
        std::cout << "Выберите действие: ";
        std::cin >> choice;

        try {
            if (choice == 1) {
                std::cout << "\nВсе товары:\n";
                for (const auto& p : products) {
                    std::cout << "  " << p->toDisplayString() << "\n";
                }
            }
            else if (choice == 2) {
                if (!user) {
                    std::cout << "Сначала создайте пользователя (пункт 5).\n";
                    continue;
                }
                if (!cart) {
                    cart = std::make_shared<Cart>(user);
                }
                int id;
                std::cout << "Введите ID товара: ";
                std::cin >> id;
                auto it = std::find_if(products.begin(), products.end(),
                    [id](const auto& p) { return p->getId() == id; });
                if (it != products.end()) {
                    cart->addItem(*it);
                    std::cout << "Товар добавлен в корзину.\n";
                }
                else {
                    std::cout << "Товар с ID=" << id << " не найден.\n";
                }
            }
            else if (choice == 3) {
                if (!cart) {
                    std::cout << "Корзина пуста.\n";
                }
                else {
                    cart->show();
                }
            }
            else if (choice == 4) {
                if (!cart || cart->isEmpty()) {
                    std::cout << "Нельзя оформить пустой заказ.\n";
                }
                else {
                    Purchase purchase(cart);
                    purchase.complete();
                    cart.reset(); // очистить корзину после заказа
                }
            }
            else if (choice == 5) {
                std::string name, email;
                std::cout << "Введите имя: ";
                std::cin.ignore();
                std::getline(std::cin, name);
                std::cout << "Введите email: ";
                std::getline(std::cin, email);
                user = std::make_shared<User>(name, email);
                std::cout << "Пользователь создан: " << user->getName() << "\n";
            }
            else if (choice == 6) {
                if (!user) {
                    std::cout << "Сначала создайте пользователя (пункт 5).\n";
                    continue;
                }
                int id, rating;
                std::cout << "Введите ID товара: ";
                std::cin >> id;
                std::cout << "Введите оценку (1-5): ";
                std::cin >> rating;
                auto it = std::find_if(products.begin(), products.end(),
                    [id](const auto& p) { return p->getId() == id; });
                if (it != products.end()) {
                    Rating r(user, *it, rating);
                    (*it)->addRating(r);
                    std::cout << "Оценка добавлена.\n";
                }
                else {
                    std::cout << "Товар не найден.\n";
                }
            }
            else if (choice == 7) {
                std::cout << "\nТовары с оценками:\n";
                for (const auto& p : products) {
                    if (p->getAverageRating() > 0) {
                        std::cout << "  " << p->toDisplayString()
                            << " | Средняя оценка: " << p->getAverageRating() << "\n";
                    }
                }
            }
            else if (choice == 8) {
                int id;
                std::cout << "Введите ID товара: ";
                std::cin >> id;
                auto it = std::find_if(products.begin(), products.end(),
                    [id](const auto& p) { return p->getId() == id; });
                if (it != products.end()) {
                    std::cout << "Найден: " << (*it)->toDisplayString() << "\n";
                }
                else {
                    std::cout << "Товар не найден.\n";
                }
            }
            else if (choice == 9) {
                std::cout << "\n=== Все данные ===\n";
                if (user) {
                    std::cout << "Пользователь: " << user->getName() << " (" << user->getEmail() << ")\n";
                }
                else {
                    std::cout << "Пользователь: не задан\n";
                }
                std::cout << "Товаров: " << products.size() << "\n";
                if (cart) {
                    std::cout << "Корзина: ";
                    cart->show();
                }
                else {
                    std::cout << "Корзина: пуста\n";
                }
            }
            else if (choice == 10) {
                demoVirtualFunctions();
            }
            else if (choice == 11) {
                demoCloning();
            }
            
            else if (choice == 12) {
                std::cout << "\n=== Сортировка товаров по цене ===\n";
                if (products.empty()) {
                    std::cout << "Список товаров пуст.\n";
                    continue;
                }
                auto sorted = products;
                std::sort(sorted.begin(), sorted.end(),
                    [](const auto& a, const auto& b) {
                        return a->getPrice() < b->getPrice();
                    });
                std::cout << "Товары по возрастанию цены:\n";
                for (const auto& p : sorted) {
                    std::cout << "  " << p->toDisplayString() << "\n";
                }
            }
            else if (choice == 13) {
                std::cout << "\n=== Поиск первого eBook ===\n";
                auto it = std::find_if(products.begin(), products.end(),
                    [](const auto& p) { return p->containsTag("ebook"); });
                if (it != products.end()) {
                    std::cout << "Найден: " << (*it)->toDisplayString() << "\n";
                }
                else {
                    std::cout << "eBook не найден.\n";
                }
            }
            else if (choice == 14) {
                std::cout << "\n=== Средняя цена всех товаров ===\n";
                double avg = averagePrice(products);
                std::cout << "Средняя цена: " << avg << "₽\n";
            }
            else if (choice == 15) {
                std::cout << "\n=== Демо: шаблонный Catalog<Product> ===\n";
                Catalog<Product> catalog;
                for (const auto& p : products) {
                    if (p->containsTag("book") || p->containsTag("ebook")) {
                        catalog.add(p);
                    }
                }
                std::cout << "Добавлено в каталог: " << catalog.size() << " товаров\n";
                std::cout << "Общая цена: " << catalog.getTotalPrice() << "₽\n";

                std::cout << "\nСортируем по убыванию цены:\n";
                catalog.sortByPrice(false);
                catalog.printAll();

                std::cout << "\nИщем товары дороже 700₽:\n";
                auto expensive = catalog.findIf([](const auto& p) {
                    return p->getPrice() > 700.0;
                    });
                for (const auto& p : expensive) {
                    std::cout << "  " << p->toDisplayString() << "\n";
                }

                std::cout << "\nТрансформируем в имена:\n";
                auto names = catalog.transform([](const auto& p) {
                    return p->getName();
                    });
                for (const auto& name : names) {
                    std::cout << "  • " << name << "\n";
                }
            }
            else if (choice == 0) {
                std::cout << "Выход.\n";
            }
            else {
                std::cout << "Неверный выбор.\n";
            }
        }
        catch (const std::exception& e) {
            std::cerr << "Ошибка: " << e.what() << "\n";
        }
    } while (choice != 0);

    return 0;
}