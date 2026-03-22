#include <iostream>
#include <vector>
#include <string>
#include <stdexcept>
#include <memory>
#include "User.h"
#include "Product.h"
#include "Book.h"
#include "Purchase.h"
#include "Rating.h"
#include "Cart.h"

// Глобальные контейнеры
std::vector<std::shared_ptr<User>> users;
std::vector<std::shared_ptr<Product>> products;
std::vector<std::shared_ptr<Rating>> ratings;
std::vector<std::unique_ptr<Purchase>> purchases;

std::shared_ptr<User> findUserById(int id) {
    auto it = std::find_if(users.begin(), users.end(),
        [id](const auto& u) { return u && u->getId() == id; });
    return (it != users.end()) ? *it : nullptr;
}

std::shared_ptr<Product> findProductById(int id) {
    auto it = std::find_if(products.begin(), products.end(),
        [id](const auto& p) { return p && p->getId() == id; });
    return (it != products.end()) ? *it : nullptr;
}

int main() {
    std::cout << "=== Книжный интернет-магазин (C++ modern) ===\n";
    int choice;

    while (true) {
        std::cout << "\nМеню:\n";
        std::cout << "1. Создать пользователя\n";
        std::cout << "2. Создать товар (обычный)\n";
        std::cout << "3. Создать книгу\n";
        std::cout << "4. Добавить товар в корзину\n";
        std::cout << "5. Оформить заказ\n";
        std::cout << "6. Поставить оценку товару\n";
        std::cout << "7. Показать все данные\n";
        std::cout << "0. Выйти\n";
        std::cout << "Выберите действие: ";
        std::cin >> choice;

        try {
            if (choice == 1) {
                int id, age;
                std::string name, email, pass;
                std::cout << "ID пользователя: "; std::cin >> id;
                std::cin.ignore(); std::getline(std::cin, name);
                std::cout << "Имя: "; std::getline(std::cin, name);
                std::cout << "Email: "; std::getline(std::cin, email);
                std::cout << "Возраст: "; std::cin >> age;
                std::cout << "Хэш пароля: "; std::cin >> pass;
                auto u = std::make_shared<User>(id, name, email, age, pass);
                users.push_back(u);
                std::cout << "Пользователь '" << name << "' создан.\n";
            }
            else if (choice == 2 || choice == 3) {
                int id;
                std::string name, desc, cat;
                double price;
                std::cout << "ID товара: "; std::cin >> id;
                std::cin.ignore(); std::getline(std::cin, name);
                std::cout << "Название: "; std::getline(std::cin, name);
                std::cout << "Описание: "; std::getline(std::cin, desc);
                std::cout << "Цена: "; std::cin >> price;
                std::cout << "Категория: "; std::cin.ignore(); std::getline(std::cin, cat);
                std::shared_ptr<Product> p;
                if (choice == 2) {
                    p = std::make_shared<Product>(id, name, desc, price, cat);
                }
                else {
                    std::string author, isbn;
                    std::cout << "Автор: "; std::getline(std::cin, author);
                    std::cout << "ISBN: "; std::getline(std::cin, isbn);
                    p = std::make_shared<Book>(id, name, author, price, isbn);
                }
                products.push_back(p);
                std::cout << "Товар '" << name << "' создан.\n";
            }
            else if (choice == 4) {
                int userId, productId;
                std::cout << "ID пользователя: "; std::cin >> userId;
                std::cout << "ID товара: "; std::cin >> productId;
                auto u = findUserById(userId);
                auto p = findProductById(productId);
                if (!u) { std::cout << "Пользователь не найден.\n"; continue; }
                if (!p) { std::cout << "Товар не найден.\n"; continue; }
                u->addToCart(p);
                std::cout << "Товар '" << p->getName() << "' добавлен в корзину пользователя '"
                    << u->getName() << "'.\n";
            }
            else if (choice == 5) {
                int userId;
                std::cout << "ID пользователя для оформления заказа: "; std::cin >> userId;
                auto u = findUserById(userId);
                if (!u) { std::cout << "Пользователь не найден.\n"; continue; }
                auto cart = u->getCart();
                if (!cart || !*cart) {
                    std::cout << "Корзина пуста.\n";
                    continue;
                }
                auto purchase = cart->checkout();
                purchases.push_back(std::move(purchase));
                std::cout << "Заказ #" << purchases.back()->getId()
                    << " оформлен на сумму " << purchases.back()->getTotalAmount() << "₽\n";
            }
            else if (choice == 6) {
                int userId, productId, score;
                std::cout << "ID пользователя: "; std::cin >> userId;
                std::cout << "ID товара: "; std::cin >> productId;
                std::cout << "Оценка (1–5): "; std::cin >> score;
                auto u = findUserById(userId);
                auto p = findProductById(productId);
                if (!u) { std::cout << "Пользователь не найден.\n"; continue; }
                if (!p) { std::cout << "Товар не найден.\n"; continue; }
                if (score < 1 || score > 5) {
                    std::cout << "Оценка должна быть от 1 до 5.\n";
                    continue;
                }
                auto r = std::make_shared<Rating>(static_cast<int>(ratings.size()) + 1, u, p, score);
                ratings.push_back(r);
                std::cout << "Оценка " << score << " поставлена товару '" << p->getName()
                    << "' от '" << u->getName() << "'.\n";
            }
            else if (choice == 7) {
                std::cout << "\n--- Все пользователи ---\n";
                for (const auto& u : users) {
                    std::cout << "[" << u->getId() << "] " << u->getName()
                        << " (" << u->getEmail() << ")\n";
                }
                std::cout << "\n--- Все товары ---\n";
                for (const auto& p : products) {
                    std::cout << *p << "\n";
                }
                std::cout << "\n--- Все заказы ---\n";
                for (size_t i = 0; i < purchases.size(); ++i) {
                    auto& p = purchases[i];
                    std::cout << "Заказ #" << p->getId()
                        << " от пользователя ID=" << p->getUser()->getId()
                        << ", сумма: " << p->getTotalAmount() << "₽\n";
                }
                std::cout << "\n--- Все оценки ---\n";
                for (const auto& r : ratings) {
                    std::cout << "Оценка " << static_cast<int>(*r)
                        << " от пользователя ID=" << r->getUser()->getId()
                        << " товару ID=" << r->getProduct()->getId() << "\n";
                }
            }
            else if (choice == 0) {
                std::cout << "Выход. Спасибо за работу!\n";
                break;
            }
            else {
                std::cout << "Неверный выбор. Попробуйте снова.\n";
            }
        }
        catch (const std::exception& e) {
            std::cout << "Ошибка: " << e.what() << "\n";
        }
    }

    return 0;
}